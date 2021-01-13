#ifndef ILI9341_CMDS_H_
#define ILI9341_CMDS_H_

#define ILI9341_SCR_W               (uint16_t)240
#define ILI9341_SCR_H               (uint16_t)320

#define ILI9341_SLPOUT              0x11
#define ILI9341_DISPOFF             0x28
#define ILI9341_DISPON              0x29
#define ILI9341_CASET               0x2A
#define ILI9341_RASET               0x2B
#define ILI9341_RAMWR               0x2C
#define ILI9341_MADCTL              0x36
#define ILI9341_COLMOD              0x3A
#define ILI9341_INTCTRL             0xF6

#define ILI9341_MADCTL_MY           0x80
#define ILI9341_MADCTL_MX           0x40
#define ILI9341_MADCTL_MV           0x20
#define ILI9341_MADCTL_ML           0x10
#define ILI9341_MADCTL_RGB          0x00
#define ILI9341_MADCTL_BGR          0x08
#define ILI9341_MADCTL_MH           0x04

#define ILI9341_COLMOD_16BIT        0x55
#define ILI9341_COLMOD_18BIT        0x66

#define ILI9341_INTCTRL_MDT00       0x00
#define ILI9341_INTCTRL_MDT01       0x01
#define ILI9341_INTCTRL_MDT10       0x02
#define ILI9341_INTCTRL_MDT11       0x03

#endif // ILI9341_CMDS_H_
