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
#include "fonts.h"
#include "rgb565-colors.h"
#include "color-scr-drv.h"
#include "imgsensor-drv.h"
#include <stdio.h>

//--------------------------------------------
extern const color_scr_drv_t scr_drv;
extern const imgsensor_drv_t cam_drv;

//--------------------------------------------
#define HLINE 320
#define VLINE 240
#define SIZE_IN_BYTES (HLINE * VLINE * 2)
#define SIZE_ROW_IN_BYTES (HLINE * 2)

//--------------------------------------------
static uint32_t row_buf[SIZE_ROW_IN_BYTES / sizeof(uint32_t)];

//--------------------------------------------
void color_scr_test(void)
{
	color_scr_init(RGB565);
	color_scr_setorientation(SCR_ORIENT_0);
	delay_ms(25);

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
	delay_ms(25);
	color_scr_printstring("Hello!", 0, 0, RGB565_MAGENTA, RGB565_WHITE, font_16x26);
	color_scr_printchar(0xA8, 20, 25, RGB565_RED, RGB565_WHITE, monotypeSorts_26ptBitmaps);
	delay_ms(1000);
	color_scr_setorientation(SCR_ORIENT_180);
	delay_ms(25);
	color_scr_printstring("Hello!", 0, 0, RGB565_GREEN, RGB565_WHITE, font_16x26);
	color_scr_printchar(0xA8, 20, 25, RGB565_BLUE, RGB565_WHITE, monotypeSorts_26ptBitmaps);
	delay_ms(1000);
	color_scr_setorientation(SCR_ORIENT_270);
	delay_ms(25);
	color_scr_printstring("Hello!", 0, 0, RGB565_MAROON, RGB565_WHITE, font_16x26);
	color_scr_printchar(0xA8, 20, 25, RGB565_YELLOW, RGB565_WHITE, monotypeSorts_26ptBitmaps);
	delay_ms(1000);
}


//--------------------------------------------
int main(void)
{
	platform_init();

#if 1
	color_scr_test();
#endif

	color_scr_init(RGB565);
	color_scr_setorientation(SCR_ORIENT_270);
	delay_ms(25);
	color_scr_fillscreen(RGB565_YELLOW);
	delay_ms(25);
	scr_drv.set_bound_rect(0, 0, HLINE, VLINE);
	scr_drv.start_memory_write();

//------
#ifdef ILI9341_8080I
// 8080I display only
// FSMC_BTR adjustment is needed in 8080I mode
// (hal_ili9341_init in hal-ili9341-8080I-16bit-fmc.c)
#if 0
	// camera => (by DMA) => screen
	// the main option
	cam_drv.init(1);
	cam_drv.init_dma_cycle();
	cam_drv.start_dma_cycle(scr_drv.get_data_addr());
	while (1);
#endif
#endif

//------
// both SPI and 8080I displays
// FSMC_BTR adjustment is needed in 8080I mode
// (hal_ili9341_init in hal-ili9341-8080I-16bit-fmc.c)
#if 1
	cam_drv.init(1);
	cam_drv.start_capture();
	scr_drv.init_dma();
	while (1)
	{
		uint32_t cnt;
		uint32_t row;
		uint16_t *row_buf16 = (uint16_t *)&row_buf[0];

		cam_drv.wait_vsync();
		for (row = 0; row < VLINE; row++)
		{
			cam_drv.wait_hsync();
			// camera => (by CPU) => memory row buffer
			for (cnt = 0; cnt < SIZE_ROW_IN_BYTES / sizeof(uint32_t); cnt++)
			{
				row_buf[cnt] = cam_drv.read_fifo();
			}
			cam_drv.disable_clock();
#if 1
			// memory row buffer => (by CPU) => screen
			for (cnt = 0; cnt < SIZE_ROW_IN_BYTES / sizeof(uint16_t); cnt++)
			{
				scr_drv.memory_write(row_buf16[cnt]);
			}
#else
			// memory row buffer => (by DMA) => screen
#ifdef ILI9341_SPI
			// All pixels in the internal capture buffer is in the Little Endian format.
			// The display ILI9341 in SPI-mode does not support the Little Endian format.
			// DMA cannot swap bytes during transfer operation.
			// So there are two possible solutions:
			// 1. Capture video in the Big Endian format => cam_drv.init(0);
			// 2. Swap bytes in the internal buffer before call DMA transfer.
			for (cnt = 0; cnt < SIZE_ROW_IN_BYTES / sizeof(uint16_t); cnt++)
			{
				row_buf16[cnt] = row_buf16[cnt] << 8 | row_buf16[cnt] >> 8;
			}
#endif
			scr_drv.write_dma((uint8_t *)&row_buf[0], SIZE_ROW_IN_BYTES);
#endif
			cam_drv.enable_clock();
		}
	}
#endif
}
