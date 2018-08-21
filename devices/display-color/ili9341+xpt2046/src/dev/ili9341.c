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
#include "hal-ili9341.h"
#include "ili9341-cmds.h"

static color_scr_mode_t scr_mode;
static uint16_t scr_width;
static uint16_t scr_height;

//--------------------------------------------
static void ili9341_command(uint8_t cmd)
{
	hal_ili9341_command();
	hal_ili9341_tx(cmd);
}

//--------------------------------------------
static void ili9341_setpixelformat(color_scr_mode_t mode)
{
	scr_mode = mode;
	hal_ili9341_command();
	hal_ili9341_tx(ILI9341_COLMOD);
	hal_ili9341_data();
	switch (scr_mode)
	{
	case RGB565:
		hal_ili9341_tx(ILI9341_COLMOD_16BIT);
		break;
	case RGB666:
		hal_ili9341_tx(ILI9341_COLMOD_18BIT);
#ifdef ILI9341_8080I
		hal_ili9341_command();
		hal_ili9341_tx(ILI9341_INTCTRL);
		hal_ili9341_data();
		hal_ili9341_tx(0x01);
		hal_ili9341_tx(ILI9341_INTCTRL_MDT11);
		hal_ili9341_tx(0x00);
#endif
		break;
	}
}

//--------------------------------------------
void ili9341_init(color_scr_mode_t mode)
{
	hal_ili9341_init();
	hal_ili9341_select();

	ili9341_command(ILI9341_SLPOUT);
	ili9341_command(ILI9341_DISPON);
	ili9341_setpixelformat(mode);

	hal_ili9341_release();
}

//--------------------------------------------
void ili9341_setorientation(scr_orient_t orientation)
{
	hal_ili9341_select();

	hal_ili9341_command();
	hal_ili9341_tx(ILI9341_MADCTL);
	hal_ili9341_data();
	switch (orientation)
	{
	case SCR_ORIENT_0:
		scr_width = ILI9341_SCR_W;
		scr_height = ILI9341_SCR_H;
		hal_ili9341_tx(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
		break;
	case SCR_ORIENT_90:
		scr_width = ILI9341_SCR_H;
		scr_height = ILI9341_SCR_W;
		hal_ili9341_tx(ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
		break;
	case SCR_ORIENT_180:
		scr_width = ILI9341_SCR_W;
		scr_height = ILI9341_SCR_H;
		hal_ili9341_tx(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
		break;
	case SCR_ORIENT_270:
		scr_width = ILI9341_SCR_H;
		scr_height = ILI9341_SCR_W;
		hal_ili9341_tx(ILI9341_MADCTL_MX | ILI9341_MADCTL_MV | ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
		break;
	}

	hal_ili9341_release();
}

//--------------------------------------------
void ili9341_setboundrect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	hal_ili9341_select();

	ili9341_command(ILI9341_CASET);
	hal_ili9341_data();
	hal_ili9341_tx(x1 >> 8);
	hal_ili9341_tx(x1 & 0xFF);
	hal_ili9341_tx(x2 >> 8);
	hal_ili9341_tx(x2 & 0xFF);

	ili9341_command(ILI9341_RASET);
	hal_ili9341_data();
	hal_ili9341_tx(y1 >> 8);
	hal_ili9341_tx(y1 & 0xFF);
	hal_ili9341_tx(y2 >> 8);
	hal_ili9341_tx(y2 & 0xFF);

	hal_ili9341_release();
}

//--------------------------------------------
void ili9341_startmemorywrite(void)
{
	hal_ili9341_select();
	ili9341_command(ILI9341_RAMWR);
	hal_ili9341_data();
}

//--------------------------------------------
void ili9341_stopmemorywrite(void)
{
	hal_ili9341_release();
}

//--------------------------------------------
void ili9341_memorywrite(uint32_t color)
{
	switch (scr_mode)
	{
	case RGB565:
#ifdef ILI9341_8080I
		hal_ili9341_tx(color);
#endif
#ifdef ILI9341_SPI
		hal_ili9341_tx(color >> 8);
		hal_ili9341_tx(color);
#endif
		break;
	case RGB666:
#ifdef ILI9341_8080I
		hal_ili9341_tx(color >> 16);
		hal_ili9341_tx(color);
#endif
#ifdef ILI9341_SPI
		hal_ili9341_tx(((color >> 12) & 0x3F) << 2);
		hal_ili9341_tx(((color >> 6) & 0x3F) << 2);
		hal_ili9341_tx((color & 0x3F) << 2);
#endif
		break;
	}
}

//--------------------------------------------
uint16_t ili9341_getwidth(void)
{
	return scr_width;
}

//--------------------------------------------
uint16_t ili9341_getheight(void)
{
	return scr_height;
}
