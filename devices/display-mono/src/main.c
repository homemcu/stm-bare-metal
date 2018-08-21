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
#include "mono-scr.h"

//--------------------------------------------
#include "fonts.h"
void mono_scr_TEST(void)
{
	mono_scr_fillscreen(MONO_SCR_PSET);
	mono_scr_flush();

	mono_scr_setorientation(SCR_ORIENT_0);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PSET, font_16x26);
	mono_scr_flush();

	mono_scr_setorientation(SCR_ORIENT_90);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PSET, font_16x26);
	mono_scr_flush();

	mono_scr_setorientation(SCR_ORIENT_180);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PSET, font_16x26);
	mono_scr_flush();

	mono_scr_setorientation(SCR_ORIENT_270);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PSET, font_16x26);
	mono_scr_flush();

	delay_ms(1000);
	mono_scr_setorientation(SCR_ORIENT_0);
	mono_scr_fillscreen(MONO_SCR_PSET);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PSET, font_4x6);
	mono_scr_flush();
	delay_ms(1000);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PRES, font_8x8);
	mono_scr_flush();
	delay_ms(1000);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PSET, font_10x16);
	mono_scr_flush();
	delay_ms(1000);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PSET, font_16x26);
	mono_scr_flush();

	delay_ms(1000);
	mono_scr_fillscreen(MONO_SCR_PSET);
	mono_scr_printstring("87", 0, 0, MONO_SCR_PSET, font_8x8);
	mono_scr_flush();

	delay_ms(1000);
	mono_scr_fillscreen(MONO_SCR_PSET);
	mono_scr_printchar('9', 2, 0, MONO_SCR_PSET, microsoftSansSerif_8ptBitmaps);
	mono_scr_flush();
	mono_scr_setorientation(SCR_ORIENT_90);
	mono_scr_printchar('9', 2, 2, MONO_SCR_PSET, microsoftSansSerif_8ptBitmaps);
	mono_scr_flush();
	mono_scr_setorientation(SCR_ORIENT_180);
	mono_scr_printchar('9', 2, 0, MONO_SCR_PSET, microsoftSansSerif_8ptBitmaps);
	mono_scr_flush();
	mono_scr_setorientation(SCR_ORIENT_270);
	mono_scr_printchar('9', 2, 2, MONO_SCR_PSET, microsoftSansSerif_8ptBitmaps);
	mono_scr_flush();

	delay_ms(1000);
	mono_scr_fillscreen(MONO_SCR_PSET);
	mono_scr_printchar('9', 0, 0, MONO_SCR_PSET, dSEG7ModernMini_12ptBitmaps);
	mono_scr_flush();

	delay_ms(1000);
	mono_scr_fillscreen(MONO_SCR_PSET);
	mono_scr_setorientation(SCR_ORIENT_0);
	mono_scr_printchar(0xA8, 10, 10, MONO_SCR_PSET, monotypeSorts_26ptBitmaps);
	mono_scr_flush();

}

//--------------------------------------------
int main(void)
{
	platform_init();
	mono_scr_init();
	mono_scr_TEST();
	return 0;
}
