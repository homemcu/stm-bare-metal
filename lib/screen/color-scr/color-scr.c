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

#include "platform.h"
#include "scr.h"
#include "color-scr-drv.h"
#include <string.h>

extern const color_scr_drv_t scr_drv;
static scr_orient_t scr_orient = SCR_ORIENT_0;

//--------------------------------------------
void color_scr_init(color_scr_mode_t mode)
{
	scr_drv.init(mode);
}

//--------------------------------------------
void color_scr_setorientation(scr_orient_t orientation)
{
	scr_orient = orientation;
	scr_drv.set_orientation(orientation);
}

//--------------------------------------------
scr_orient_t color_scr_getorientation(void)
{
	return scr_orient;
}

//--------------------------------------------
uint16_t color_scr_getwidth(void)
{
	return scr_drv.width();
}

//--------------------------------------------
uint16_t color_scr_getheight(void)
{
	return scr_drv.height();
}

//--------------------------------------------
void color_scr_drawpixel(uint16_t x, uint16_t y, uint32_t color)
{
	scr_drv.set_bound_rect(x, y, x + 1, y + 1);
	scr_drv.start_memory_write();
	scr_drv.memory_write(color);
	scr_drv.stop_memory_write();
}

//--------------------------------------------
void color_scr_drawhline(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
	uint16_t cnt;

	scr_drv.set_bound_rect(x, y, x + len, y);
	scr_drv.start_memory_write();
	for (cnt = 0; cnt < len; cnt++)
	{
		scr_drv.memory_write(color);
	}
	scr_drv.stop_memory_write();
}

//--------------------------------------------
void color_scr_drawvline(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
	uint16_t cnt;

	scr_drv.set_bound_rect(x, y, x, y + len);
	scr_drv.start_memory_write();
	for (cnt = 0; cnt < len; cnt++)
	{
		scr_drv.memory_write(color);
	}
	scr_drv.stop_memory_write();
}

//--------------------------------------------
void color_scr_drawcross(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
	color_scr_drawhline(x - len, y, len * 2, color);
	color_scr_drawvline(x, y - len, len * 2, color);
}

//--------------------------------------------
void color_scr_fillrect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
	uint16_t cnt;

	for (cnt = 0; cnt < y2 - y1; cnt++)
	{
		color_scr_drawhline(x1, y1 + cnt, x2 - x1, color);
	}
}

//--------------------------------------------
void color_scr_fillscreen(uint32_t color)
{
	color_scr_fillrect(0, 0, color_scr_getwidth(), color_scr_getheight(), color);
}

//--------------------------------------------
void color_scr_printchar(uint8_t ch, uint16_t x, uint16_t y, uint32_t char_color, uint32_t back_color, FLASH_MEMORY_DECLARE(uint8_t, *font))
{
	uint8_t ch_byte_width;
	uint8_t ch_pix_width;
	uint8_t ch_pix_height;
	uint8_t ch_from;
	uint8_t ch_to;
	uint8_t ch_col_byte_cnt;
	uint8_t ch_col_bit_cnt;
	uint8_t ch_row_cnt;
	uint16_t ch_offset;
	uint8_t ch_byte;
	uint8_t x_beg;

	ch_pix_width = FLASH_MEMORY_READ_BYTE(font++);
	ch_pix_height = FLASH_MEMORY_READ_BYTE(font++);
	ch_from = FLASH_MEMORY_READ_BYTE(font++);
	ch_to = FLASH_MEMORY_READ_BYTE(font++);

	if ((ch < ch_from) || (ch > ch_to))
	{
		return;
	}

	scr_drv.set_bound_rect(x, y, x + ch_pix_width - 1, y + ch_pix_height - 1);
	scr_drv.start_memory_write();

	ch_byte_width = (ch_pix_width % 8) ? (ch_pix_width / 8) + 1 : (ch_pix_width / 8);
	ch_offset = (ch_byte_width * ch_pix_height) * (ch - ch_from);

	font += ch_offset;

	for (ch_row_cnt = 0; ch_row_cnt < ch_pix_height; ch_row_cnt++, y++)
	{
		for (ch_col_byte_cnt = 0, x_beg = x; ch_col_byte_cnt < ch_byte_width; ch_col_byte_cnt++, font++)
		{
			ch_byte = FLASH_MEMORY_READ_BYTE(font);
			for (ch_col_bit_cnt = 0;
				(ch_col_bit_cnt < 8) && (ch_col_bit_cnt < (ch_pix_width - ch_col_byte_cnt * 8));
				ch_col_bit_cnt++, x_beg++, ch_byte >>= 1)
			{
				if (ch_byte & 0x01)
				{
					scr_drv.memory_write(char_color);
				}
				else
				{
					scr_drv.memory_write(back_color);
				}
			}
		}
	}

	scr_drv.stop_memory_write();
}

//--------------------------------------------
void color_scr_printstring(const char *st, uint16_t x, uint16_t y, uint32_t char_color, uint32_t back_color, FLASH_MEMORY_DECLARE(uint8_t, *font))
{
	uint8_t len;
	uint8_t cnt;
	uint8_t x_size;

	x_size = FLASH_MEMORY_READ_BYTE(font);
	len = (uint8_t)strlen(st);

	for (cnt = 0; cnt < len; cnt++)
	{
		color_scr_printchar(*st++, x + cnt * x_size, y, char_color, back_color, font);
	}
}
