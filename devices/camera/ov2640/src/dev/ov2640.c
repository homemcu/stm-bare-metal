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

#include "hal-imgsensor-dcmi.h"
#include "hal-imgsensor-i2c.h"
#include "ov2640-cmds.h"
#include <stdio.h>

//--------------------------------------------
// Mandatory initialization
static const uint8_t ov2640_init_regs[][2] =
{
	{ 0xFF, 0x00 },
	{ 0x2C, 0xFF },
	{ 0x2E, 0xDF },
	{ 0xFF, 0x01 },
	{ 0x11, 0x01 }, // CLK = XVCLK / 2
	{ 0x12, 0x00 }, // Resolution selectors for UXGA
	{ 0x3D, 0x38 }, // SVGA
	{ 0xFF, 0x00 },
	{ 0xE0, 0x04 }, // reset DVP
	{ 0xC0, 0xC8 }, // Image horizontal size HSIZE[10:3] 1600
	{ 0xC1, 0x96 }, // Image vertical size VSIZE[10:3] 1200
	{ 0x86, 0x3D }, // Module enable: DCW, SDE, UV_ADJ, UV_AVG, CMX
	{ 0x5A, 0x50 }, // OUTW[7:0] (real/4) 80*4=320
	{ 0x5B, 0x3C }, // OUTH[7:0] (real/4) 60*4=240
	{ 0xE0, 0x00 }, // reset none
	{ 0x05, 0x00 }, // enable DSP
	{ 0xDA, 0x08 }, // RGB565
};

//--------------------------------------------
// Additional initialization only improves the quality of the image
static const uint8_t ov2640_ext_init_regs[][2] =
{
	{ 0xFF, 0x01 },
	{ 0x3C, 0x32 },
	{ 0x09, 0x02 },
	{ 0x04, 0x28 },
	{ 0x13, 0xE5 },
	{ 0x14, 0x48 },
	{ 0x2C, 0x0C },
	{ 0x33, 0x78 },
	{ 0x3A, 0x33 },
	{ 0x3B, 0xFB },
	{ 0x43, 0x11 },
	{ 0x16, 0x10 },
	{ 0x39, 0x02 },
	{ 0x35, 0x88 },
	{ 0x22, 0x0A },
	{ 0x37, 0x40 },
	{ 0x23, 0x00 },
	{ 0x34, 0xA0 },
	{ 0x06, 0x02 },
	{ 0x07, 0xC0 },
	{ 0x0D, 0xB7 },
	{ 0x0E, 0x01 },
	{ 0x4C, 0x00 },
	{ 0x4A, 0x81 },
	{ 0x21, 0x99 },
	{ 0x24, 0x40 },
	{ 0x25, 0x38 },
	{ 0x26, 0x82 },
	{ 0x5C, 0x00 },
	{ 0x63, 0x00 },
	{ 0x46, 0x22 },
	{ 0x47, 0x00 },
	{ 0x0C, 0x3A },
	{ 0x5D, 0x55 },
	{ 0x5E, 0x7D },
	{ 0x5F, 0x7D },
	{ 0x60, 0x55 },
	{ 0x61, 0x70 },
	{ 0x62, 0x80 },
	{ 0x7C, 0x05 },
	{ 0x20, 0x80 },
	{ 0x28, 0x30 },
	{ 0x6C, 0x00 },
	{ 0x6D, 0x80 },
	{ 0x6E, 0x00 },
	{ 0x70, 0x02 },
	{ 0x71, 0x94 },
	{ 0x73, 0xC1 },
	{ 0x12, 0x00 },
	{ 0x5A, 0x57 },
	{ 0x4E, 0x00 },
	{ 0x50, 0x9C },
};

#if 0
static void read_registers(void)
{
	uint8_t data;
	uint32_t cnt;

	printf("Read the camera registers:\n");
	printf("reg address = data\n");
	hal_imgsensor_write_register(BANK_SEL, 0x00);
	for (cnt = 0; cnt <= 0xFE; cnt++)
	{
		hal_imgsensor_read_register(cnt, &data);
		printf("bank0 0x%02X = 0x%02X\n", cnt, data);
	}
	hal_imgsensor_write_register(BANK_SEL, 0x01);
	for (cnt = 0; cnt <= 0xFE; cnt++)
	{
		hal_imgsensor_read_register(cnt, &data);
		printf("bank1 0x%02X = 0x%02X\n", cnt, data);
	}
}
#endif

//--------------------------------------------
#if 0
static void getchipclock(void)
{ 
	uint32_t fclk;
	uint8_t  reg; 
	 
	hal_imgsensor_write_register(BANK_SEL, 0x01);
	hal_imgsensor_read_register(0x3D, &reg); 
	reg = reg & 0x3F; 
	reg = 0x40 - reg; 
	fclk = 12 * reg / 2; // fXCLK = 12MHz
	 
	hal_imgsensor_read_register(0x11, &reg); 
	reg = reg & 0x3F; 
	reg = (reg + 1) * 2; 
	fclk = fclk / reg; 
	printf("chip clock is %d MHz\n", fclk);
} 
#endif

//--------------------------------------------
void ov2640_init(uint8_t little_endian)
{
	uint8_t reg;
	uint32_t cnt;

	hal_imgsensor_init_capture();
	hal_imgsensor_init_i2c();

	hal_imgsensor_hard_reset();
	printf("Initializing the image sensor...\n");

	// Soft reset
	if (hal_imgsensor_write_register(BANK_SEL, 0x01))
	{
		printf("Failed to initialize the image sensor.\n");
		printf("Check hardware connection, pullup resistors on i2c wires, etc...\n");
		printf("Execution is stopped.\n");
		while (1);
	}
	hal_imgsensor_write_register(COM7, COM7_SRST);
	delay_ms(10);
	// Mandatory initialization
	for (cnt = 0; cnt < (sizeof(ov2640_init_regs) / sizeof(ov2640_init_regs[0])); cnt++)
	{
		hal_imgsensor_write_register(ov2640_init_regs[cnt][0], ov2640_init_regs[cnt][1]);
	}
	// Additional initialization only improves the quality of the image
	for (cnt = 0; cnt < (sizeof(ov2640_ext_init_regs) / sizeof(ov2640_ext_init_regs[0])); cnt++)
	{
		hal_imgsensor_write_register(ov2640_ext_init_regs[cnt][0], ov2640_ext_init_regs[cnt][1]);
	}
	if (little_endian)
	{
		// U Y V Y
		hal_imgsensor_write_register(BANK_SEL, 0x00);
		hal_imgsensor_read_register(IMAGE_MODE, &reg);
		reg |= IMAGE_MODE_LBYTE_FIRST;
		hal_imgsensor_write_register(IMAGE_MODE, reg);
		hal_imgsensor_read_register(CTRL0, &reg);
		reg &= ~CTRL0_VFIRST;
		hal_imgsensor_write_register(CTRL0, reg);
	}
	printf("Initialization is completed successfully.\n");

#if 0
	// Color bar
	hal_imgsensor_write_register(BANK_SEL, 0x01);
	hal_imgsensor_read_register(COM7, &reg);
	reg |= COM7_COLOR_BAR_TEST;
	hal_imgsensor_write_register(COM7, reg);
#endif
#if 0
	// Registers
	read_registers();
#endif
#if 0
	// Get chip clock
	getchipclock();
#endif
}

//--------------------------------------------
uint32_t ov2640_read_dma_buf(uint8_t *rxbuf, uint32_t length)
{
	uint32_t res;

	res = hal_imgsensor_read_dma_buf(rxbuf, length, 100);
	hal_imgsensor_stop_dma();
	return res;
}
