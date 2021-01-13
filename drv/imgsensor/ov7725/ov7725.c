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
#include "ov7725-cmds.h"
#include <stdio.h>

//--------------------------------------------
// Mandatory initialization
static const uint8_t ov7725_init_regs[][2] =
{
	{ COM7,          0x46 }, // QVGA, RGB565, RGB
	{ DSP_CTRL4,     0x00 }, // RGB
	{ COM4,          0x01 }, // PLL 1x
	{ CLKRC,         0xC0 }, // fCLK = 24MHz
	{ HSTART,        0x3F },
	{ HSIZE,         0x50 },
	{ VSTART,        0x03 },
	{ VSIZE,         0x78 },
	{ HREF,          0x00 },
	{ COM3,          0x00 },
};

//--------------------------------------------
// Additional initialization only improves the quality of the image
static const uint8_t ov7725_ext_init_regs[][2] =
{
	{ EXHCH,         0x00},
	{ TGT_B,         0x7F},
	{ FIXGAIN,       0x09},
	{ AWB_CTRL0,     0xE0},
	{ DSP_CTRL1,     0xFF},
	{ DSP_CTRL2,     0x0F},
	{ DSP_CTRL3,     0x00},
	{ DSPAUTO,       0xFF},
	{ COM8,          0xF0},
	{ COM6,          0xC5},
	{ COM9,          0x11},
	{ BDBASE,        0x7F},
	{ DBSTEP,        0x03},
	{ AEW,           0x96},
	{ AEB,           0x64},
	{ VPT,           0xA1},
	{ EXHCL,         0x00},
	{ AWB_CTRL3,     0xAA},
	{ COM8,          0xFF},

	// Gamma
	{ GAM1,          0x0C},
	{ GAM2,          0x16},
	{ GAM3,          0x2A},
	{ GAM4,          0x4E},
	{ GAM5,          0x61},
	{ GAM6,          0x6F},
	{ GAM7,          0x7B},
	{ GAM8,          0x86},
	{ GAM9,          0x8E},
	{ GAM10,         0x97},
	{ GAM11,         0xA4},
	{ GAM12,         0xAF},
	{ GAM13,         0xC5},
	{ GAM14,         0xD7},
	{ GAM15,         0xE8},

	{ SLOP,          0x20},
	{ EDGE_TRSHLD,   0x05},
	{ EDGE_UPPER,    0x03},
	{ EDGE_LOWER,    0x00},
	{ DNSOFF,        0x01},

	{ MTX1,          0xB0},
	{ MTX2,          0x9D},
	{ MTX3,          0x13},
	{ MTX4,          0x16},
	{ MTX5,          0x7B},
	{ MTX6,          0x91},
	{ MTX_CTRL,      0x1E},

	{ BRIGHT,        0x08},
	{ CNTRST,        0x30},
	{ UVAD_J0,       0x81},
	{ SDE,           0x06},

	// For 30 fps/60Hz
	{ DM_LNL,        0x00},
	{ DM_LNH,        0x00},
	{ BDBASE,        0x7F},
	{ DBSTEP,        0x03},
	
	// Lens Correction, should be tuned with real camera module
	{ LC_RADI,       0x10},
	{ LC_COEF,       0x10},
	{ LC_COEFB,      0x14},
	{ LC_COEFR,      0x17},
	{ LC_CTR,        0x05},
	{ COM5,          0xF5}, //0x65
};

//--------------------------------------------
#if 0
static void read_registers(void)
{
	uint8_t data;
	uint32_t cnt;

	printf("Read the camera registers:\n");
	printf("reg address = data\n");
	for (cnt = 0; cnt <= 0xFF; cnt++)
	{
		hal_imgsensor_read_register(cnt, &data);
		printf("0x%02X = 0x%02X\n", cnt, data);
	}
}
#endif

//--------------------------------------------
void ov7725_init(uint8_t little_endian)
{
	uint8_t reg;
	uint32_t cnt;

	hal_imgsensor_init_capture();
	hal_imgsensor_init_clock();
	hal_imgsensor_init_i2c();

	hal_imgsensor_hard_reset();
	printf("Initializing the image sensor...\n");

	// Soft reset
	if (hal_imgsensor_write_register(COM7, 0x80))
	{
		printf("Failed to initialize the image sensor.\n");
		printf("Check hardware connection, pullup resistors on i2c wires, etc...\n");
		printf("Execution is stopped.\n");
		while (1);
	}
	delay_ms(10);
	// Mandatory initialization
	for (cnt = 0; cnt < (sizeof(ov7725_init_regs) / sizeof(ov7725_init_regs[0])); cnt++)
	{
		hal_imgsensor_write_register(ov7725_init_regs[cnt][0], ov7725_init_regs[cnt][1]);
	}
	// Additional initialization only improves the quality of the image
	for (cnt = 0; cnt < (sizeof(ov7725_ext_init_regs) / sizeof(ov7725_ext_init_regs[0])); cnt++)
	{
		hal_imgsensor_write_register(ov7725_ext_init_regs[cnt][0], ov7725_ext_init_regs[cnt][1]);
	}
	if (little_endian)
	{
		hal_imgsensor_read_register(COM3, &reg);
		reg |= 0x20;
		hal_imgsensor_write_register(COM3, reg);
	}
	printf("Initialization is completed successfully.\n");

#if 0
	// Color bar
	hal_imgsensor_read_register(COM3, &reg);
	reg |= 0x01;
	hal_imgsensor_write_register(COM3, reg);
#endif
#if 0
	// Registers
	read_registers();
#endif
}

//--------------------------------------------
uint32_t ov7725_read_dma_buf(uint8_t *rxbuf, uint32_t length)
{
	uint32_t res;

	res = hal_imgsensor_read_dma_buf(rxbuf, length, 100);
	hal_imgsensor_stop_dma();
	return res;
}
