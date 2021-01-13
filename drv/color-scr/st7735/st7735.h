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

#ifndef ST7735_H_
#define ST7735_H_

void st7735_init(color_scr_mode_t mode);
void st7735_setorientation(scr_orient_t orientation);
void st7735_setboundrect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void st7735_startmemorywrite(void);
void st7735_stopmemorywrite(void);
void st7735_memorywrite(uint32_t color);
uint16_t st7735_getwidth(void);
uint16_t st7735_getheight(void);

#endif // ST7735_H_
