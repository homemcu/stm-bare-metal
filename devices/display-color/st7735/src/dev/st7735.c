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
#include "hal-st7735.h"
#include "st7735-cmds.h"

static color_scr_mode_t scr_mode;
static uint16_t scr_width;
static uint16_t scr_height;

//--------------------------------------------
static void st7735_command(uint8_t cmd)
{
	hal_st7735_command();
	hal_st7735_tx(cmd);
}

//--------------------------------------------
static void st7735_setpixelformat(color_scr_mode_t mode)
{
	scr_mode = mode;
	hal_st7735_command();
	hal_st7735_tx(ST7735_COLMOD);
	hal_st7735_data();
	switch (scr_mode)
	{
	case RGB444:
		hal_st7735_tx(ST7735_COLMOD_12BIT);
		break;
	case RGB565:
		hal_st7735_tx(ST7735_COLMOD_16BIT);
		break;
	case RGB666:
		hal_st7735_tx(ST7735_COLMOD_18BIT);
		break;
	}
}

//--------------------------------------------
void st7735_init(color_scr_mode_t mode)
{
	hal_st7735_init();
	hal_st7735_select();

	st7735_command(ST7735_SLPOUT);
	st7735_command(ST7735_DISPON);
	st7735_setpixelformat(mode);

	hal_st7735_release();
}

//--------------------------------------------
void st7735_setorientation(scr_orient_t orientation)
{
	hal_st7735_select();

	hal_st7735_command();
	hal_st7735_tx(ST7735_MADCTL);
	hal_st7735_data();
	switch (orientation)
	{
	case SCR_ORIENT_0:
		scr_width = ST7735_SCR_W;
		scr_height = ST7735_SCR_H;
		hal_st7735_tx(ST7735_MADCTL_RGB);
		break;
	case SCR_ORIENT_90:
		scr_width = ST7735_SCR_H;
		scr_height = ST7735_SCR_W;
		hal_st7735_tx(ST7735_MADCTL_MV | ST7735_MADCTL_MX | ST7735_MADCTL_RGB);
		break;
	case SCR_ORIENT_180:
		scr_width = ST7735_SCR_W;
		scr_height = ST7735_SCR_H;
		hal_st7735_tx(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB);
		break;
	case SCR_ORIENT_270:
		scr_width = ST7735_SCR_H;
		scr_height = ST7735_SCR_W;
		hal_st7735_tx(ST7735_MADCTL_MV | ST7735_MADCTL_MY | ST7735_MADCTL_RGB);
		break;
	}

	hal_st7735_release();
}

//--------------------------------------------
void st7735_setboundrect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	hal_st7735_select();

	st7735_command(ST7735_CASET);
	hal_st7735_data();
	hal_st7735_tx(x1 >> 8);
	hal_st7735_tx(x1 & 0xFF);
	hal_st7735_tx(x2 >> 8);
	hal_st7735_tx(x2 & 0xFF);

	st7735_command(ST7735_RASET);
	hal_st7735_data();
	hal_st7735_tx(y1 >> 8);
	hal_st7735_tx(y1 & 0xFF);
	hal_st7735_tx(y2 >> 8);
	hal_st7735_tx(y2 & 0xFF);

	hal_st7735_release();
}

//--------------------------------------------
void st7735_startmemorywrite(void)
{
	hal_st7735_select();
	st7735_command(ST7735_RAMWR);
	hal_st7735_data();
}

//--------------------------------------------
void st7735_stopmemorywrite(void)
{
	hal_st7735_release();
}

//--------------------------------------------
void st7735_memorywrite(uint32_t color)
{
	switch (scr_mode)
	{
	case RGB444:
		hal_st7735_tx(color >> 24);
		hal_st7735_tx(color >> 16);
		hal_st7735_tx(color >> 8);
		hal_st7735_tx(color);
		break;
	case RGB565:
		hal_st7735_tx(color >> 8);
		hal_st7735_tx(color);
		break;
	case RGB666:
		hal_st7735_tx(color >> 16);
		hal_st7735_tx(color >> 8);
		hal_st7735_tx(color);
		break;
	}
}

//--------------------------------------------
uint16_t st7735_getwidth(void)
{
	return scr_width;
}

//--------------------------------------------
uint16_t st7735_getheight(void)
{
	return scr_height;
}
