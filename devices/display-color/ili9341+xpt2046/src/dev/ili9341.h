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

#ifndef ILI9341_H_
#define ILI9341_H_

void ili9341_init(color_scr_mode_t mode);
void ili9341_setorientation(scr_orient_t orientation);
void ili9341_setboundrect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ili9341_startmemorywrite(void);
void ili9341_stopmemorywrite(void);
void ili9341_memorywrite(uint32_t color);
uint16_t ili9341_getwidth(void);
uint16_t ili9341_getheight(void);

#endif // ILI9341_H_
