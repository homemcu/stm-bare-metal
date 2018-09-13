/*
* Copyright (c) 2018 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under 
* the terms of GNU General Public License version 2 
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef COLOR_SCR_H_
#define COLOR_SCR_H_

void color_scr_init(color_scr_mode_t mode);
void color_scr_setorientation(scr_orient_t orientation);
scr_orient_t color_scr_getorientation(void);
uint16_t color_scr_getwidth(void);
uint16_t color_scr_getheight(void);
void color_scr_drawpixel(uint16_t x, uint16_t y, uint32_t color);
void color_scr_drawhline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
void color_scr_drawvline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
void color_scr_drawcross(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
void color_scr_fillrect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
void color_scr_fillscreen(uint32_t color);
void color_scr_printchar(uint8_t ch, uint16_t x, uint16_t y, uint32_t char_color, uint32_t back_color, FLASH_MEMORY_DECLARE(uint8_t, *font));
void color_scr_printstring(const char *st, uint16_t x, uint16_t y, uint32_t char_color, uint32_t back_color, FLASH_MEMORY_DECLARE(uint8_t, *font));

#endif // COLOR_SCR_H_
