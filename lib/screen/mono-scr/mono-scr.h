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
#ifndef MONO_SCR_H_
#define MONO_SCR_H_

void mono_scr_init(void);
void mono_scr_flush(void);
void mono_scr_setorientation(scr_orient_t orientation);
void mono_scr_drawpixel(uint8_t x, uint8_t y, mono_scr_color_t color);
void mono_scr_fillscreen(mono_scr_color_t color);
void mono_scr_printchar(uint8_t ch, uint8_t x, uint8_t y, mono_scr_color_t color, FLASH_MEMORY_DECLARE(uint8_t, *font));
void mono_scr_printstring(const char *st, uint8_t x, uint8_t y, mono_scr_color_t color, FLASH_MEMORY_DECLARE(uint8_t, *font));

#endif // MONO_SCR_H_
