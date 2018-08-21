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
#include "mono-scr-drv.h"
#include <string.h>

extern const mono_scr_drv_t scr_drv;
static scr_orient_t scr_orientation = SCR_ORIENT_0;
static uint8_t *scr_buf;

//--------------------------------------------
void mono_scr_init(void)
{
	scr_drv.init();
	scr_buf = scr_drv.buf();
}

//--------------------------------------------
void mono_scr_flush(void)
{
	scr_drv.flush();
}

//--------------------------------------------
void mono_scr_setorientation(scr_orient_t orientation)
{
	scr_orientation = orientation;
}

//--------------------------------------------
void mono_scr_drawpixel(uint8_t x, uint8_t y, mono_scr_color_t color)
{
	uint16_t offset;
	uint8_t temp;

	switch (scr_orientation)
	{
	case SCR_ORIENT_90:
		temp = y;
		y = x;
		x = scr_drv.width - temp - 1;
		break;
	case SCR_ORIENT_180:
		x = scr_drv.width - x - 1;
		y = scr_drv.height - y - 1;
		break;
	case SCR_ORIENT_270:
		temp = x;
		x = y;
		y = scr_drv.height - temp - 1;
		break;
	}

	offset = x + (y / 8) * scr_drv.width;

	if (offset >= scr_drv.height * scr_drv.width / 8)
	{
		return;
	}

	switch (color)
	{
	case MONO_SCR_PSET:
		scr_buf[offset] |=  (1 << (y & 0x07));
		break;
	case MONO_SCR_PRES:
		scr_buf[offset] &= ~(1 << (y & 0x07));
		break;
	}
}

//--------------------------------------------
void mono_scr_fillscreen(mono_scr_color_t color)
{
	uint8_t pattern;
	if (color == MONO_SCR_PSET)
	{
		pattern = 0;
	}
	else
	{
		pattern = 0xFF;
	}
	memset(scr_buf, pattern, (scr_drv.height * scr_drv.width) / 8);
}

//--------------------------------------------
void mono_scr_printchar(uint8_t ch, uint8_t x, uint8_t y, mono_scr_color_t color, FLASH_MEMORY_DECLARE(uint8_t, *font))
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
					mono_scr_drawpixel(x_beg, y, color);
				}
				else
				{
					mono_scr_drawpixel(x_beg, y, color == MONO_SCR_PSET ? MONO_SCR_PRES : MONO_SCR_PSET);
				}
			}
		}
	}
}

//--------------------------------------------
void mono_scr_printstring(const char *st, uint8_t x, uint8_t y, mono_scr_color_t color, FLASH_MEMORY_DECLARE(uint8_t, *font))
{
	uint8_t len;
	uint8_t cnt;
	uint8_t x_size;

	x_size = FLASH_MEMORY_READ_BYTE(font);
	len = (uint8_t)strlen(st);

	for (cnt = 0; cnt < len; cnt++)
	{
		mono_scr_printchar(*st++, x + cnt * x_size, y, color, font);
	}
}
