/* DC8000 hw reg*/

#define FRAMEBUFFER_CONFIG                                          0x1518
#define FRAMEBUFFER_ADDRESS                                         0x1400
#define FRAMEBUFFER_STRIDE                                          0x1408
#define HDISPLAY                                                    0x1430
#define HSYNC                                                       0x1438
#define VDISPLAY                                                    0x1440
#define VSYNC                                                       0x1448
#define PANEL_CONFIG                                                0x1418
#define DPI_CONFIG                                                  0x14B8
#define CURSOR_ADDRESS                                              0x146C
#define CURSOR_CONFIG                                               0x1468
#define CURSOR_LOCATION                                             0x1470
#define CURSOR_BACKGROUND                                           0x1474
#define CURSOR_FOREGROUND                                           0x1478
#define FRAMEBUFFER_UPLANAR_ADDRESS                                 0x1530
#define FRAMEBUFFER_VPLANAR_ADDRESS                                 0x1538
#define FRAMEBUFFER_USTRIDE                                         0x1800
#define FRAMEBUFFER_VSTRIDE                                         0x1808
#define INDEXCOLOR_TABLEINDEX                                       0x1818
#define INDEXCOLOR_TABLEDATA                                        0x1820
#define FRAMEBUFFER_SIZE                                            0x1810
#define FRAMEBUFFER_SCALEFACTORX                                    0x1828
#define FRAMEBUFFER_SCALEFACTORY                                    0x1830
#define FRAMEBUFFER_SCALEFCONFIG                                    0x1520
#define HORIFILTER_KERNELINDEX                                      0x1838
#define HORIFILTER_KERNEL                                           0x1A00
#define VERTIFILTER_KERNELINDEX                                     0x1A08
#define VERTIFILTER_KERNEL                                          0x1A10
#define FRAMEBUFFER_INITIALOFFSET                                   0x1A20
#define FRAMEBUFFER_COLORKEY                                        0x1508
#define FRAMEBUFFER_COLORHIGHKEY                                    0x1510
#define FRAMEBUFFER_BGCOLOR                                         0x1528
#define FRAMEBUFFER_CLEARVALUE                                      0x1A18
#define DISPLAY_INTRENABLE                                          0x1480
#define INT_STATE                                                   0x147C
#define PANEL_DEST_ADDRESS                                          0x14F0
#define MEM_DEST_ADDRESS                                            0x14E8
#define DEST_CONFIG                                                 0x14F8
#define DEST_STRIDE                                                 0x1500
#define DBI_CONFIG                                                  0x1488
#define AQHICLOCKCONTROL                                            0x0000
#define OVERLAY_CONFIG                                              0x1540
#define OVERLAY_STRIDE                                              0x1600
#define OVERLAY_USTRIDE                                             0x18C0
#define OVERLAY_VSTRIDE                                             0x1900
#define OVERLAY_TL                                                  0x1640
#define OVERLAY_BR                                                  0x1680
#define OVERLAY_ALPHA_BLEND_CONFIG                                  0x1580
#define OVERLAY_SRC_GLOBAL_COLOR                                    0x16C0
#define OVERLAY_DST_GLOBAL_COLOR                                    0x1700
#define OVERLAY_CLEAR_VALUE                                         0x1940
#define OVERLAY_SIZE                                                0x17C0
#define OVERLAY_COLOR_KEY                                           0x1740
#define OVERLAY_COLOR_KEY_HIGH                                      0x1780
#define OVERLAY_ADDRESS                                             0x15C0
#define OVERLAY_UPLANAR_ADDRESS                                     0x1840
#define OVERLAY_VPLANAR_ADDRESS                                     0x1880
#define OVERLAY_SCALE_CONFIG                                        0x1C00
#define OVERLAY_SCALE_FACTOR_X                                      0x1A40
#define OVERLAY_SCALE_FACTOR_Y                                      0x1A80
#define OVERLAY_HORI_FILTER_KERNEL_INDEX                            0x1AC0
#define OVERLAY_HORI_FILTER_KERNEL                                  0x1B00
#define OVERLAY_VERTI_FILTER_KERNEL_INDEX                           0x1B40
#define OVERLAY_VERTI_FILTER_KERNEL                                 0x1B80
#define OVERLAY_INITIAL_OFFSET                                      0x1BC0
#define GAMMA_EX_INDEX                                              0x1CF0
#define GAMMA_EX_DATA                                               0x1CF8
#define GAMMA_EX_ONE_DATA                                           0x1D80
#define GAMMA_INDEX                                                 0x1458
#define GAMMA_DATA                                                  0x1460
#define DISPLAY_DITHER_TABLE_LOW                                    0x1420
#define DISPLAY_DITHER_TABLE_HIGH                                   0x1428
#define DISPLAY_DITHER_CONFIG                                       0x1410
