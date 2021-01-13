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
#include "ov7670-cmds.h"
#include <stdio.h>

//--------------------------------------------
// Mandatory initialization
static const uint8_t ov7670_init_regs[][2] =
{
	// Clock scale: 3 = 15fps
	//              2 = 20fps
	//              1 = 30fps
	{ REG_CLKRC,     0x01 }, // clock scale (30 fps)
	{ REG_TSLB,      0x04 },
	{ REG_HSTART,    0x13 },
	{ REG_HSTOP,     0x01 },
	{ REG_HREF,      0xB6 },
	{ REG_COM7,      COM7_FMT_QVGA | COM7_YUV }, // QVGA-YUV
	{ REG_RGB444,    0x00 },
	{ REG_COM15,     COM15_R00FF }, // R00FF
};

//--------------------------------------------
// Additional initialization only improves the quality of the image
static const uint8_t ov7670_ext_init_regs[][2] =
{
	{ REG_VSTART, 0x02 },
	{ REG_VSTOP, 0x7a },
	{ REG_VREF, 0x0a },
	{ REG_COM3, 0 },
	{ REG_COM14, 0 },

	// Mystery scaling numbers
	{ REG_SCALING_XSC, 0x3a },
	{ REG_SCALING_YSC, 0x35 },
	{ 0x72, 0x11 },
	{ 0x73, 0xf0 },
	{ 0xa2, 0x02 },
	{ REG_COM10, 0x0 }, 

	// Gamma curve values
	{ 0x7a, 0x20 },
	{ 0x7b, 0x10 },
	{ 0x7c, 0x1e },
	{ 0x7d, 0x35 },
	{ 0x7e, 0x5a },
	{ 0x7f, 0x69 },
	{ 0x80, 0x76 },
	{ 0x81, 0x80 },
	{ 0x82, 0x88 },
	{ 0x83, 0x8f },
	{ 0x84, 0x96 },
	{ 0x85, 0xa3 },
	{ 0x86, 0xaf },
	{ 0x87, 0xc4 },
	{ 0x88, 0xd7 },
	{ 0x89, 0xe8 },

	// AGC and AEC parameters.  Note we start by disabling those features,
	// then turn them only after tweaking the values.
	{ REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT },
	{ REG_GAIN, 0 },
	{ REG_AECH, 0 },
	{ REG_COM4, 0x40 }, // magic reserved bit
	{ REG_COM9, 0x18 }, // 4x gain + magic rsvd bit
	{ REG_BD50MAX, 0x05 },
	{ REG_BD60MAX, 0x07 },
	{ REG_AEW, 0x95 },
	{ REG_AEB, 0x33 },
	{ REG_VPT, 0xe3 },
	{ REG_HAECC1, 0x78 },
	{ REG_HAECC2, 0x68 },
	{ 0xa1, 0x03 }, // magic
	{ REG_HAECC3, 0xd8 },
	{ REG_HAECC4, 0xd8 },
	{ REG_HAECC5, 0xf0 },
	{ REG_HAECC6, 0x90 },
	{ REG_HAECC7, 0x94 },
	{ REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_BFILT|COM8_AGC|COM8_AEC },

	// Almost all of these are magic "reserved" values
	{ REG_COM5, 0x61 },
	{ REG_COM6, 0x4b },
	{ 0x16, 0x02 },
	{ REG_MVFP, 0x07 },
	{ 0x21, 0x02 },
	{ 0x22, 0x91 },
	{ 0x29, 0x07 },
	{ 0x33, 0x0b },
	{ 0x35, 0x0b },
	{ 0x37, 0x1d },
	{ 0x38, 0x71 },
	{ 0x39, 0x2a },
	{ REG_COM12, 0x78 },
	{ 0x4d, 0x40 },
	{ 0x4e, 0x20 },
	{ REG_GFIX, 0x00 },
	{ 0x6b, 0x4a },
	{ 0x74, 0x10 },
	{ 0x8d, 0x4f },
	{ 0x8e, 0x00 },
	{ 0x8f, 0x00 },
	{ 0x90, 0x00 },
	{ 0x91, 0x00 },
	{ 0x96, 0x00 },
	{ 0x9a, 0x00 },
	{ 0xb0, 0x84 },
	{ 0xb1, 0x0c },
	{ 0xb2, 0x0e },
	{ 0xb3, 0x82 },
	{ 0xb8, 0x0a },

	// More reserved magic, some of which tweaks white balance
	{ 0x43, 0x0a },
	{ 0x44, 0xf0 },
	{ 0x45, 0x34 },
	{ 0x46, 0x58 },
	{ 0x47, 0x28 },
	{ 0x48, 0x3a },
	{ 0x59, 0x88 },
	{ 0x5a, 0x88 },
	{ 0x5b, 0x44 },
	{ 0x5c, 0x67 },
	{ 0x5d, 0x49 },
	{ 0x5e, 0x0e },
	{ 0x6c, 0x0a },
	{ 0x6d, 0x55 },
	{ 0x6e, 0x11 },
	{ 0x6f, 0x9f }, // "9e for advance AWB"
	{ 0x6a, 0x40 },
	{ REG_BLUE, 0x40 },
	{ REG_RED, 0x60 },
	{ REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_BFILT|COM8_AGC|COM8_AEC|COM8_AWB },

	// Matrix coefficients
	{ 0x4f, 0x80 },
	{ 0x50, 0x80 },
	{ 0x51, 0x00 },
	{ 0x52, 0x22 },
	{ 0x53, 0x5e },
	{ 0x54, 0x80 },
	{ 0x58, 0x9e },
	{ REG_COM16, COM16_AWBGAIN },
	{ REG_EDGE, 0 },
	{ 0x75, 0x05 },
	{ 0x76, 0xe1 },
	{ 0x4c, 0x00 },
	{ 0x77, 0x01 },
	{ REG_COM13, 0xc3 },
	{ 0x4b, 0x09 },
	{ 0xc9, 0x60 },
	{ REG_COM16, 0x38 },
	{ 0x56, 0x40 },
	{ 0x34, 0x11 },
	{ REG_COM11, COM11_EXP | COM11_HZAUTO | COM11_NIGHT },
	{ 0xa4, 0x88 },
	{ 0x96, 0x00 },
	{ 0x97, 0x30 },
	{ 0x98, 0x20 },
	{ 0x99, 0x30 },
	{ 0x9a, 0x84 },
	{ 0x9b, 0x29 },
	{ 0x9c, 0x03 },
	{ 0x9d, 0x4c },
	{ 0x9e, 0x3f },
	{ 0x78, 0x04 },

	// Extra-weird stuff.  Some sort of multiplexor register
	{ 0x79, 0x01 },
	{ 0xc8, 0xf0 },
	{ 0x79, 0x0f },
	{ 0xc8, 0x00 },
	{ 0x79, 0x10 },
	{ 0xc8, 0x7e },
	{ 0x79, 0x0a },
	{ 0xc8, 0x80 },
	{ 0x79, 0x0b },
	{ 0xc8, 0x01 },
	{ 0x79, 0x0c },
	{ 0xc8, 0x0f },
	{ 0x79, 0x0d },
	{ 0xc8, 0x20 },
	{ 0x79, 0x09 },
	{ 0xc8, 0x80 },
	{ 0x79, 0x02 },
	{ 0xc8, 0xc0 },
	{ 0x79, 0x03 },
	{ 0xc8, 0x40 },
	{ 0x79, 0x05 },
	{ 0xc8, 0x30 },
	{ 0x79, 0x26 },

	{ REG_COM9, 0x38 },	// 16x gain ceiling; 0x8 is reserved bit
	{ 0x4f, 0xb3 },		// "matrix coefficient 1"
	{ 0x50, 0xb3 },		// "matrix coefficient 2"
	{ 0x51, 0x00 },		// vb
	{ 0x52, 0x3d },		// "matrix coefficient 4"
	{ 0x53, 0xa7 },		// "matrix coefficient 5"
	{ 0x54, 0xe4 },		// "matrix coefficient 6"
	{ REG_COM13, COM13_GAMMA | COM13_UVSAT},
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
void ov7670_init(uint8_t little_endian)
{
	uint8_t reg;
	uint32_t cnt;

	hal_imgsensor_init_capture();
	hal_imgsensor_init_clock();
	hal_imgsensor_init_i2c();

	hal_imgsensor_hard_reset();
	printf("Initializing the image sensor...\n");

	// Soft reset
	if (hal_imgsensor_write_register(REG_COM7, COM7_RESET))
	{
		printf("Failed to initialize the image sensor.\n");
		printf("Check hardware connection, pullup resistors on i2c wires, etc...\n");
		printf("Execution is stopped.\n");
		while (1);
	}
	delay_ms(10);
	// Mandatory initialization
	for (cnt = 0; cnt < (sizeof(ov7670_init_regs) / sizeof(ov7670_init_regs[0])); cnt++)
	{
		hal_imgsensor_write_register(ov7670_init_regs[cnt][0], ov7670_init_regs[cnt][1]);
	}
	// Additional initialization only improves the quality of the image
	for (cnt = 0; cnt < (sizeof(ov7670_ext_init_regs) / sizeof(ov7670_ext_init_regs[0])); cnt++)
	{
		hal_imgsensor_write_register(ov7670_ext_init_regs[cnt][0], ov7670_ext_init_regs[cnt][1]);
	}

	hal_imgsensor_read_register(REG_TSLB, &reg);
	reg &= ~TSLB_YLAST;
	hal_imgsensor_write_register(REG_TSLB, reg);
	hal_imgsensor_read_register(REG_COM13, &reg);
	reg |= COM13_UVSWAP;
	hal_imgsensor_write_register(REG_COM13, reg);

	printf("Initialization is completed successfully.\n");

#if 0
	// Color bar
	hal_imgsensor_read_register(REG_SCALING_XSC, &reg);
	reg &= ~TEST_PATTERN_0;
	hal_imgsensor_write_register(REG_SCALING_XSC, reg);
	hal_imgsensor_read_register(REG_SCALING_YSC, &reg);
	reg |= TEST_PATTERN_1;
	hal_imgsensor_write_register(REG_SCALING_YSC, reg);
#endif
#if 0
	// Registers
	read_registers();
#endif
}

//--------------------------------------------
uint32_t ov7670_read_dma_buf(uint8_t *rxbuf, uint32_t length)
{
	uint32_t res;

	res = hal_imgsensor_read_dma_buf(rxbuf, length, 100);
	hal_imgsensor_stop_dma();
	return res;
}
