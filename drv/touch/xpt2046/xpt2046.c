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
#include "hal-xpt2046.h"
#include "xpt2046-cmds.h"

//--------------------------------------------
#define TOTAL_ITERATIONS   (uint8_t)8

//--------------------------------------------
void xpt2046_init(void)
{
	hal_xpt2046_init();
}

//--------------------------------------------
uint16_t xpt2046_xpos(void)
{
	uint8_t cnt;
	uint16_t buf[TOTAL_ITERATIONS];
	uint32_t sum;

	sum = 0;
	for (cnt = 0; cnt < TOTAL_ITERATIONS; cnt++)
	{
		buf[cnt] = hal_xpt2046_read(XPT2046_CMD_XPOS);
		sum += buf[cnt];
	}
	return sum / TOTAL_ITERATIONS;
}

//--------------------------------------------
uint16_t xpt2046_ypos(void)
{
	uint8_t cnt;
	uint16_t buf[TOTAL_ITERATIONS];
	uint32_t sum;

	sum = 0;
	for (cnt = 0; cnt < TOTAL_ITERATIONS; cnt++)
	{
		buf[cnt] = hal_xpt2046_read(XPT2046_CMD_YPOS);
		sum += buf[cnt];
	}
	return sum / TOTAL_ITERATIONS;
}

//--------------------------------------------
uint8_t xpt2046_zpos(void)
{
	return hal_xpt2046_read_int();
}
