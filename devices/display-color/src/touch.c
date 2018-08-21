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
#include "touch-drv.h"

extern const touch_drv_t tch_drv;

//--------------------------------------------
#include "scr.h"
#include "color-scr.h"
#include "rgb565-colors.h"
#include "fonts.h"


//--------------------------------------------
void scr_init(void)
{
	color_scr_init(RGB565);
	color_scr_setorientation(SCR_ORIENT_270);
	color_scr_fillscreen(RGB565_WHITE);
	color_scr_printstring("Please click the cross:", 10, 10, RGB565_BLUE, RGB565_WHITE, font_8x8);
}

//--------------------------------------------
void touch_init(void)
{
	tch_drv.init();
}

//--------------------------------------------
static void touch_applycalibration_convertcoords(uint16_t *x_tch, uint16_t *y_tch, uint8_t size)
{
	uint16_t width;
	uint16_t height;
	uint16_t tmp;
	uint32_t cnt;

	width = color_scr_getwidth();
	height = color_scr_getheight();

	switch(color_scr_getorientation())
	{
	case SCR_ORIENT_90:
		for (cnt = 0; cnt < size; cnt ++)
		{
			tmp = x_tch[cnt];
			x_tch[cnt] = y_tch[cnt];
			y_tch[cnt] = height - tmp - 1;
		}
		break;
	case SCR_ORIENT_180:
		for (cnt = 0; cnt < size; cnt ++)
		{
			x_tch[cnt] = width - x_tch[cnt] - 1;
			y_tch[cnt] = height - y_tch[cnt] - 1;
		}
		break;
	case SCR_ORIENT_270:
		for (cnt = 0; cnt < size; cnt ++)
		{
			tmp = y_tch[cnt];
			y_tch[cnt] = x_tch[cnt];
			x_tch[cnt] = width - tmp - 1;
		}
		break;
	}
}

//--------------------------------------------
static void touch_initcalibration_convertcoords(uint16_t *x_tch, uint16_t *y_tch, uint8_t size)
{
	uint16_t width;
	uint16_t height;
	uint16_t tmp;
	uint32_t cnt;

	width = color_scr_getwidth();
	height = color_scr_getheight();

	switch(color_scr_getorientation())
	{
	case SCR_ORIENT_90:
		for (cnt = 0; cnt < size; cnt ++)
		{
			tmp = y_tch[cnt];
			y_tch[cnt] = x_tch[cnt];
			x_tch[cnt] = height - tmp - 1;
		}
		break;
	case SCR_ORIENT_180:
		for (cnt = 0; cnt < size; cnt ++)
		{
			x_tch[cnt] = width - x_tch[cnt] - 1;
			y_tch[cnt] = height - y_tch[cnt] - 1;
		}
		break;
	case SCR_ORIENT_270:
		for (cnt = 0; cnt < size; cnt ++)
		{
			tmp = x_tch[cnt];
			x_tch[cnt] = y_tch[cnt];
			y_tch[cnt] = width - tmp - 1;
		}
		break;
	}
}


//--------------------------------------------
// The touch screen calibration algorithm by Carlos E. Vidales is used
// https://www.embedded.com/design/system-integration/4023968/How-To-Calibrate-Touch-Screens

static float An;
static float Bn;
static float Cn;
static float Dn;
static float En;
static float Fn;

//--------------------------------------------
static void touch_initcalibration(uint16_t *x_scr, uint16_t *y_scr, uint16_t *x_tch, uint16_t *y_tch)
{
	float Div;

	touch_initcalibration_convertcoords(x_scr, y_scr, 3);

	Div = ((float)(int16_t)(x_tch[0] - x_tch[2]) * (float)(int16_t)(y_tch[1] - y_tch[2])) - 
		  ((float)(int16_t)(x_tch[1] - x_tch[2]) * (float)(int16_t)(y_tch[0] - y_tch[2]));

	An = (((float)(int16_t)(x_scr[0] - x_scr[2]) * (float)(int16_t)(y_tch[1] - y_tch[2])) -
		  ((float)(int16_t)(x_scr[1] - x_scr[2]) * (float)(int16_t)(y_tch[0] - y_tch[2]))) / Div;

	Bn = (((float)(int16_t)(x_tch[0] - x_tch[2]) * (float)(int16_t)(x_scr[1] - x_scr[2])) -
		  ((float)(int16_t)(x_scr[0] - x_scr[2]) * (float)(int16_t)(x_tch[1] - x_tch[2]))) / Div;

	Cn = (((float)x_tch[2] * (float)x_scr[1] - (float)x_tch[1] * (float)x_scr[2]) * (float)y_tch[0] +
		  ((float)x_tch[0] * (float)x_scr[2] - (float)x_tch[2] * (float)x_scr[0]) * (float)y_tch[1] +
		  ((float)x_tch[1] * (float)x_scr[0] - (float)x_tch[0] * (float)x_scr[1]) * (float)y_tch[2]) / Div;

	Dn = (((float)(int16_t)(y_scr[0] - y_scr[2]) * (float)(int16_t)(y_tch[1] - y_tch[2])) -
		  ((float)(int16_t)(y_scr[1] - y_scr[2]) * (float)(int16_t)(y_tch[0] - y_tch[2]))) / Div;
    
	En = (((float)(int16_t)(x_tch[0] - x_tch[2]) * (float)(int16_t)(y_scr[1] - y_scr[2])) -
		  ((float)(int16_t)(y_scr[0] - y_scr[2]) * (float)(int16_t)(x_tch[1] - x_tch[2]))) / Div;

	Fn = (((float)x_tch[2] * (float)y_scr[1] - (float)x_tch[1] * (float)y_scr[2]) * (float)y_tch[0] +
		  ((float)x_tch[0] * (float)y_scr[2] - (float)x_tch[2] * (float)y_scr[0]) * (float)y_tch[1] +
		  ((float)x_tch[1] * (float)y_scr[0] - (float)x_tch[0] * (float)y_scr[1]) * (float)y_tch[2]) / Div;
}

//--------------------------------------------
static void touch_applycalibration(uint16_t *x_scr, uint16_t *y_scr, uint16_t *x_tch, uint16_t *y_tch)
{

	*x_scr = (uint16_t)((An * (float)(*x_tch)) + (Bn * (float)(*y_tch)) + Cn);
	*y_scr = (uint16_t)((Dn * (float)(*x_tch)) + (En * (float)(*y_tch)) + Fn);
	touch_applycalibration_convertcoords(x_scr, y_scr, 1);
}

//--------------------------------------------
void touch_calibrate(void)
{
	uint16_t width;
	uint16_t height;
	uint32_t cnt;

	uint16_t x_scr[4];
	uint16_t y_scr[4];
	uint16_t x_tch[4];
	uint16_t y_tch[4];

	width = color_scr_getwidth();
	height = color_scr_getheight();

	x_scr[0] = width / 4;
	y_scr[0] = height / 4;
	x_scr[1] = width - width / 4;
	y_scr[1] = height / 4;
	x_scr[2] = width - width / 4;
	y_scr[2] = height - height / 4;

	for (cnt = 0; cnt < 3; cnt++)
	{
		color_scr_drawcross((uint16_t)x_scr[cnt], (uint16_t)y_scr[cnt], 10, RGB565_RED);
		for (;;)
		{
			if (!tch_drv.zpos())
			{
				x_tch[cnt] = tch_drv.xpos();
				y_tch[cnt] = tch_drv.ypos();
				color_scr_drawcross(x_scr[cnt], y_scr[cnt], 10, RGB565_WHITE);
				delay_ms(300);
				break;
			}
			delay_ms(10);
		}
	}
	touch_initcalibration(&x_scr[0], &y_scr[0], &x_tch[0], &y_tch[0]);
}

//--------------------------------------------
void touch_testcalibration(void)
{
	uint16_t width;
	uint16_t height;

	uint16_t x_tch;
	uint16_t y_tch;
	uint16_t x_scr;
	uint16_t y_scr;

	width = color_scr_getwidth();
	height = color_scr_getheight();

	x_scr = width / 2;
	y_scr = height / 2;

	color_scr_drawcross(x_scr, y_scr, 10, RGB565_BLACK);
	for (;;)
	{
		if (!tch_drv.zpos())
		{
			x_tch = tch_drv.xpos();
			y_tch = tch_drv.ypos();
			color_scr_drawcross(x_scr, y_scr, 10, RGB565_WHITE);
			touch_applycalibration(&x_scr, &y_scr, &x_tch, &y_tch);
			break;
		}
		delay_ms(10);
	}
}

//--------------------------------------------
void touch_loop(void)
{
	uint16_t x_scr;
	uint16_t y_scr;
	uint16_t x_tch;
	uint16_t y_tch;
	uint32_t end_ms;
	uint32_t now_ms;
	scr_orient_t orient;
	uint32_t color;

	scr_init();
	touch_init();
	touch_calibrate();
	touch_testcalibration();
	color_scr_fillscreen(RGB565_WHITE);
	orient = SCR_ORIENT_0;
	color = RGB565_RED;
	for (;;)
	{
		color_scr_setorientation(orient);
		color_scr_fillscreen(RGB565_WHITE);
		color_scr_printstring("Draw on me, please", 10, 10, color, RGB565_WHITE, font_8x8);
		for (now_ms = get_platform_counter(), end_ms = now_ms + 5000; end_ms > now_ms; now_ms = get_platform_counter())
		{
			if (!tch_drv.zpos())
			{
				x_tch = tch_drv.xpos();
				y_tch = tch_drv.ypos();
				touch_applycalibration(&x_scr, &y_scr, &x_tch, &y_tch);
				color_scr_drawpixel(x_scr, y_scr, color);
			}
			delay_ms(1);
		}
		switch (orient)
		{
		case SCR_ORIENT_0:
			orient = SCR_ORIENT_90;
			color = RGB565_ORANGE;
			break;
		case SCR_ORIENT_90:
			orient = SCR_ORIENT_180;
			color = RGB565_MAGENTA;
			break;
		case SCR_ORIENT_180:
			orient = SCR_ORIENT_270;
			color = RGB565_MAROON;
			break;
		case SCR_ORIENT_270:
			orient = SCR_ORIENT_0;
			color = RGB565_RED;
			break;
		}
	}
}
