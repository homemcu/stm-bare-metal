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
#include "color-scr.h"
#include "touch.h"

//--------------------------------------------
#include "fonts.h"
#include "rgb565-colors.h"
#include "rgb666-colors.h"
#define FRAME_WIDTH    10

void color_scr_TEST(void)
{
	uint8_t cnt;
	uint16_t width;
	uint16_t height;

	color_scr_init(RGB565);
	color_scr_setorientation(SCR_ORIENT_0);

	color_scr_fillscreen(RGB565_BLACK);
	color_scr_fillscreen(RGB565_WHITE);
	color_scr_fillscreen(RGB565_RED);
	color_scr_fillscreen(RGB565_LIME);
	color_scr_fillscreen(RGB565_BLUE);
	color_scr_fillscreen(RGB565_WHITE);
	color_scr_printstring("Hello!", 0, 0, RGB565_ORANGE, RGB565_WHITE, font_16x26);
	color_scr_printchar(0xA8, 20, 25, RGB565_LIME, RGB565_WHITE, monotypeSorts_26ptBitmaps);
	delay_ms(1000);
	color_scr_setorientation(SCR_ORIENT_90);
	color_scr_printstring("Hello!", 0, 0, RGB565_MAGENTA, RGB565_WHITE, font_16x26);
	color_scr_printchar(0xA8, 20, 25, RGB565_RED, RGB565_WHITE, monotypeSorts_26ptBitmaps);
	delay_ms(1000);
	color_scr_setorientation(SCR_ORIENT_180);
	color_scr_printstring("Hello!", 0, 0, RGB565_GREEN, RGB565_WHITE, font_16x26);
	color_scr_printchar(0xA8, 20, 25, RGB565_BLUE, RGB565_WHITE, monotypeSorts_26ptBitmaps);
	delay_ms(1000);
	color_scr_setorientation(SCR_ORIENT_270);
	color_scr_printstring("Hello!", 0, 0, RGB565_MAROON, RGB565_WHITE, font_16x26);
	color_scr_printchar(0xA8, 20, 25, RGB565_YELLOW, RGB565_WHITE, monotypeSorts_26ptBitmaps);
	delay_ms(1000);

	color_scr_setorientation(SCR_ORIENT_0);
	width = color_scr_getwidth();
	height = color_scr_getheight();
	color_scr_fillscreen(RGB565_WHITE);
	for (cnt = 0; cnt < FRAME_WIDTH; cnt++)
	{
		color_scr_drawhline(0, cnt, width, RGB565_RED);
	}
	for (cnt = 0; cnt < FRAME_WIDTH; cnt++)
	{
		color_scr_drawvline(cnt, 0, height, RGB565_RED);
	}
	for (cnt = FRAME_WIDTH; cnt > 0; cnt--)
	{
		color_scr_drawhline(0, height - cnt, width, RGB565_RED);
	}
	for (cnt = FRAME_WIDTH; cnt > 0; cnt--)
	{
		color_scr_drawvline(width - cnt, 0, height, RGB565_RED);
	}
	delay_ms(1000);

	color_scr_init(RGB666);
	color_scr_setorientation(SCR_ORIENT_0);
	color_scr_fillscreen(RGB666_WHITE);
	color_scr_printstring("Hello!", 0, 0, RGB666_RED, RGB666_WHITE, font_4x6);
	delay_ms(1000);
	color_scr_setorientation(SCR_ORIENT_90);
	color_scr_fillscreen(RGB666_WHITE);
	color_scr_printstring("Hello!", 0, 0, RGB666_BLUE, RGB666_WHITE, font_8x8);
	delay_ms(1000);
	color_scr_setorientation(SCR_ORIENT_180);
	color_scr_fillscreen(RGB666_WHITE);
	color_scr_printstring("Hello!", 0, 0, RGB666_GREEN, RGB666_WHITE, font_10x16);
	delay_ms(1000);
	color_scr_setorientation(SCR_ORIENT_270);
	color_scr_fillscreen(RGB666_WHITE);
	color_scr_printstring("Hello!", 0, 0, RGB666_BLACK, RGB666_WHITE, font_16x26);
	delay_ms(1000);

	color_scr_setorientation(SCR_ORIENT_0);
	color_scr_fillscreen(RGB666_WHITE);
	color_scr_printchar('9', 0, 0, RGB666_BLACK, RGB666_WHITE, dSEG7ModernMini_12ptBitmaps);
	color_scr_printchar(0xA8, 10, 10, RGB666_RED, RGB666_WHITE, monotypeSorts_26ptBitmaps);
	delay_ms(1000);
}

//--------------------------------------------
int main(void)
{
	platform_init();
	color_scr_TEST();
	touch_loop();
	return 0;
}
