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
#include "hal-ssd1306.h"
#include "ssd1306-cmds.h"

static uint8_t buf[(SSD1306_SCR_W * SSD1306_SCR_H) / 8];

//--------------------------------------------
void ssd1306_init(void)
{
	hal_ssd1306_init();

	// Display OFF
	hal_ssd1306_command(SSD1306_DISP_OFF);

	// Set clock divide ratio and oscillator frequency
	// bits[3:0] defines the divide ratio of the display clocks (bits[3:0] + 1)
	// bits[7:4] set the oscillator frequency (Fosc), frequency increases with the value of these bits
	// 0xF0 value gives maximum frequency (maximum Fosc without divider)
	// 0x0F value gives minimum frequency (minimum Fosc divided by 16)
	// The higher display frequency decreases image flickering but increases current consumption and vice versa
	hal_ssd1306_command(SSD1306_CLOCKDIV);
	hal_ssd1306_command(0xF0);

	// Set multiplex ratio (visible lines)
	hal_ssd1306_command(SSD1306_SETMUX);
	hal_ssd1306_command(SSD1306_SCR_H - 1);

	// Set display offset (offset of first line from the top of display)
	hal_ssd1306_command(SSD1306_SETOFFS);
	hal_ssd1306_command(0x00); // Offset: 0

	// Set display start line (first line displayed)
	hal_ssd1306_command(SSD1306_STARTLINE | 0x00); // Start line: 0

	// Set memory addressing mode
	// 0x00 - horizontal
	// 0x01 - vertical
	// 0x02 - page (reset state)
	hal_ssd1306_command(SSD1306_MEM_MODE);
	hal_ssd1306_command(0x00);

	// Set COM pins hardware configuration
	// bit[4]: reset - sequential COM pin configuration
	//         set   - alternative COM pin configuration (reset value)
	// bit[5]: reset - disable COM left/right remap (reset value)
	//         set   - enable COM left/right remap
#if defined SSD1306_128_32
	hal_ssd1306_command(SSD1306_COM_HW);
	hal_ssd1306_command(0x02);
#endif
#if defined SSD1306_128_64
	hal_ssd1306_command(SSD1306_COM_HW);
	hal_ssd1306_command(0x12);
#endif

	// Set contrast control
	hal_ssd1306_command(SSD1306_CONTRAST);
	hal_ssd1306_command(0x7F); // Contrast: middle level

	// Set Vcomh deselect level, values: 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70
	// This value affects on contrast level
	hal_ssd1306_command(SSD1306_VCOMH);
	hal_ssd1306_command(0x30); // ~0.83V x Vcc

	// Set segment re-map (X coordinate)
	hal_ssd1306_command(SSD1306_SEG_NORM);

	// Set COM output scan direction (Y coordinate)
	hal_ssd1306_command(SSD1306_COM_NORM);

	// Disable entire display ON
	hal_ssd1306_command(SSD1306_EDOFF); // Display follows RAM content

	// Disable display inversion
	hal_ssd1306_command(SSD1306_INV_OFF); // Normal display mode

	// Enable charge pump generator
	hal_ssd1306_command(SSD1306_CHGPUMP);
	hal_ssd1306_command(0x14);

	// Display ON
	hal_ssd1306_command(SSD1306_DISP_ON);
}

//--------------------------------------------
void ssd1306_flush(void)
{
	uint16_t cnt;

	// 10.1.4 Set Column Address (21h)
	hal_ssd1306_command(SSD1306_SET_COL);
	hal_ssd1306_command(0x00);
	hal_ssd1306_command(SSD1306_SCR_W - 1);
	// 10.1.5 Set Page Address (22h)
	hal_ssd1306_command(SSD1306_SET_PAGE);
	hal_ssd1306_command(0x00);
	hal_ssd1306_command((SSD1306_SCR_H / 8) - 1);

	hal_ssd1306_startdata();
	for (cnt = 0; cnt < sizeof(buf); cnt++)
	{
		hal_ssd1306_data(buf[cnt]);
	}
	hal_ssd1306_stopdata();
}

//--------------------------------------------
uint8_t* ssd1306_getbufaddr(void)
{
	return buf;
}
