// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 Nuvoton Technology Corp.
 */

#include <linux/delay.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/thermal.h>
#include <linux/mfd/ma35d0-sys.h>

#include "../thermal_core.h"
#include "../thermal_hwmon.h"

/* Temperature sesnor register bit mask definitions */
#define TSEN_DATA_VALID		BIT(31)
#define TSEN_POWER_DOWN		BIT(28)
#define TSEN_DATA		GENMASK(27, 16)
#define TSEN_DATA_SHIFT		16
#define TSEN_REF1		GENMASK(15, 8)
#define TSEN_REF0		GENMASK(7, 0)

struct ma35d0_tmperature_sensor {
	struct device *dev;
	struct thermal_zone_device *th_dev;
	enum thermal_device_mode mode;
	struct regmap *regmap;
	unsigned long last_jiffies;
	int last_temp;
};

static inline void ma35d0_tsen_power_on(struct ma35d0_tmperature_sensor *sensor)
{
	int reg;

	regmap_read(sensor->regmap, REG_SYS_TSENSRFCR, &reg);
	reg &= ~TSEN_POWER_DOWN;
	regmap_write(sensor->regmap, REG_SYS_TSENSRFCR, reg);
}

static inline void ma35d0_tsen_power_off(struct ma35d0_tmperature_sensor *sensor)
{
	int reg;

	regmap_read(sensor->regmap, REG_SYS_TSENSRFCR, &reg);
	reg |= TSEN_POWER_DOWN;
	regmap_write(sensor->regmap, REG_SYS_TSENSRFCR, reg);
}

/*
 *	linear trendline for following table is Temp = Value * 274.35 / 4096 - 93.33
 */
static int ma35d0_tsen_get_temp(struct thermal_zone_device *th, int *temp)
{
	struct ma35d0_tmperature_sensor *sensor = thermal_zone_device_priv(th);
	int reg;

	if (sensor->mode != THERMAL_DEVICE_ENABLED)
		return -EAGAIN;

	regmap_read(sensor->regmap, REG_SYS_TSENSRFCR, &reg);
	if (!(reg & TSEN_DATA_VALID)) {
		/*
		 * The MA35 temperature sensor requires 350 ms to complete a temperature
		 * conversion. If If the current temperature reading is taken less than
		 * 355 ms after the previous one, the previously read temperature value
		 * will be returned.
		 */
		if (jiffies - sensor->last_jiffies < 355) {
			*temp = sensor->last_temp;
			return 0;
		}
		return -EAGAIN;
	}

	// clear valid bit
	regmap_write(sensor->regmap, REG_SYS_TSENSRFCR, reg);

	reg  = (reg & TSEN_DATA) >> TSEN_DATA_SHIFT;
	*temp = (reg * 27435 / 4096 - 9333) / 100;
	sensor->last_jiffies = jiffies;
	sensor->last_temp = *temp;
	return 0;
}


#ifdef CONFIG_PM_SLEEP
static int ma35d0_tsen_suspend(struct device *dev)
{
	struct ma35d0_tmperature_sensor *sensor = dev_get_drvdata(dev);

	ma35d0_tsen_power_off(sensor);
	sensor->mode = THERMAL_DEVICE_DISABLED;
	return 0;
}

static int ma35d0_tsen_resume(struct device *dev)
{
	struct ma35d0_tmperature_sensor *sensor = dev_get_drvdata(dev);

	ma35d0_tsen_power_on(sensor);
	sensor->mode = THERMAL_DEVICE_ENABLED;
	return 0;
}
#endif /* CONFIG_PM_SLEEP */

SIMPLE_DEV_PM_OPS(ma35d0_tsen_pm_ops, ma35d0_tsen_suspend, ma35d0_tsen_resume);

static const struct thermal_zone_device_ops ma35d0_tz_ops = {
	.get_temp	= ma35d0_tsen_get_temp,
};

static const struct of_device_id ma35d0_tsen_of_match[] = {
	{ .compatible = "nuvoton,ma35d0-tsen" },
	{}
};
MODULE_DEVICE_TABLE(of, ma35d0_tsen_of_match);

static int ma35d0_tsen_probe(struct platform_device *pdev)
{
	struct ma35d0_tmperature_sensor *sensor;
	int ret;

	if (!pdev->dev.of_node) {
		dev_err(&pdev->dev, "Device tree node not found\n");
		return -EINVAL;
	}

	sensor = devm_kzalloc(&pdev->dev, sizeof(*sensor), GFP_KERNEL);
	if (!sensor)
		return -ENOMEM;

	platform_set_drvdata(pdev, sensor);

	sensor->dev = &pdev->dev;

	/* Populate sensor */
	sensor->regmap  = syscon_regmap_lookup_by_phandle(pdev->dev.of_node, "nuvoton,ma35d0-sys");
	if (IS_ERR(sensor->regmap)) {
		dev_err(&pdev->dev, "Failed to get SYS register base\n");
		return -ENODEV;
	}

	sensor->th_dev = devm_thermal_of_zone_register(&pdev->dev,
						       0, sensor,
						       &ma35d0_tz_ops);
	if (IS_ERR(sensor->th_dev)) {
		dev_err(&pdev->dev, "Thermal zone sensor register failed\n");
		ret = PTR_ERR(sensor->th_dev);
		return ret;
	}

	/* Enable temperature sensor */
	ma35d0_tsen_power_on(sensor);

	/*
	 * Thermal_zone doesn't enable hwmon as default,
	 * enable it here
	 */
	sensor->th_dev->tzp->no_hwmon = false;
	ret = thermal_add_hwmon_sysfs(sensor->th_dev);
	if (ret) {
		dev_err(&pdev->dev, "Add hwmon sysfs failed\n");
		return ret;
	}
	sensor->mode = THERMAL_DEVICE_ENABLED;

	return 0;
}

static int ma35d0_tsen_remove(struct platform_device *pdev)
{
	struct ma35d0_tmperature_sensor *sensor = platform_get_drvdata(pdev);

	ma35d0_tsen_power_off(sensor);
	thermal_remove_hwmon_sysfs(sensor->th_dev);

	return 0;
}

static struct platform_driver ma35d0_tsen_driver = {
	.driver = {
		.name	= "ma35d0_tsen",
		.pm     = &ma35d0_tsen_pm_ops,
		.of_match_table = ma35d0_tsen_of_match,
	},
	.probe		= ma35d0_tsen_probe,
	.remove		= ma35d0_tsen_remove,
};
module_platform_driver(ma35d0_tsen_driver);

MODULE_DESCRIPTION("Nuvoton MA35D0 Temperature Sensor Driver");
MODULE_LICENSE("GPL v2");
