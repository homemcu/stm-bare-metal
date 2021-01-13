#ifndef ST7735_CMDS_H_
#define ST7735_CMDS_H_

#define ST7735_SCR_W                (uint16_t)128
#define ST7735_SCR_H                (uint16_t)160

#define ST7735_SLPOUT               0x11
#define ST7735_DISPON               0x29
#define ST7735_CASET                0x2A
#define ST7735_RASET                0x2B
#define ST7735_RAMWR                0x2C
#define ST7735_COLMOD               0x3A
#define ST7735_MADCTL               0x36

#define ST7735_MADCTL_MY            0x80
#define ST7735_MADCTL_MX            0x40
#define ST7735_MADCTL_MV            0x20
#define ST7735_MADCTL_ML            0x10
#define ST7735_MADCTL_RGB           0x00
#define ST7735_MADCTL_BGR           0x08
#define ST7735_MADCTL_MH            0x04

#define ST7735_COLMOD_12BIT         0x03
#define ST7735_COLMOD_16BIT         0x05
#define ST7735_COLMOD_18BIT         0x06

#endif // ST7735_CMDS_H_
