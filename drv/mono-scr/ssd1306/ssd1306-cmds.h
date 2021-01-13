#ifndef SSD1306_CMDS_H_
#define SSD1306_CMDS_H_

#ifdef SSD1306_128_64
#define SSD1306_SCR_W           (uint8_t)128
#define SSD1306_SCR_H           (uint8_t)64
#endif
#ifdef SSD1306_128_32
#define SSD1306_SCR_W           (uint8_t)128
#define SSD1306_SCR_H           (uint8_t)32
#endif

#define SSD1306_MEM_MODE        0x20
#define SSD1306_SET_COL         0x21
#define SSD1306_SET_PAGE        0x22
#define SSD1306_STARTLINE       0x40
#define SSD1306_CONTRAST        0x81
#define SSD1306_CHGPUMP         0x8D
#define SSD1306_SEG_NORM        0xA0
#define SSD1306_EDOFF           0xA4
#define SSD1306_INV_OFF         0xA6
#define SSD1306_SETMUX          0xA8
#define SSD1306_DISP_OFF        0xAE
#define SSD1306_DISP_ON         0xAF
#define SSD1306_COM_NORM        0xC0
#define SSD1306_SETOFFS         0xD3
#define SSD1306_CLOCKDIV        0xD5
#define SSD1306_COM_HW          0xDA
#define SSD1306_VCOMH           0xDB

#endif // SSD1306_CMDS_H_
