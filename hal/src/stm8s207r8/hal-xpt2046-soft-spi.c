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

//--------------------------------------------
#define	PIN_SCK         0   // PG0 --> T_CLK
#define	PIN_CS          1   // PG1 --> T_CS
#define	PIN_MOSI        2   // PG2 --> T_DIN
#define	PIN_MISO        3   // PG3 <-- T_DO
#define	PIN_INT         4   // PG4 <-- T_IRQ

//--------------------------------------------
void hal_xpt2046_init(void)
{
	PG_DDR |= (1 << PIN_SCK) | (1 << PIN_CS) | (1 << PIN_MOSI);
	PG_CR1 |= (1 << PIN_SCK) | (1 << PIN_CS) | (1 << PIN_MOSI) | (1 << PIN_MISO) | (1 << PIN_INT);

	PG_ODR |= (1 << PIN_CS);     // CS = 1
}

//--------------------------------------------
uint16_t hal_xpt2046_read(uint8_t cmd)
{
	uint8_t cnt;
	uint8_t cnt_conv;
	uint16_t data;

	if (cmd & 0x08)
	{
		cnt_conv = 8;
	}
	else
	{
		cnt_conv = 12;
	}

	PG_ODR &= ~(1 << PIN_SCK);  // SCK  = 0
	PG_ODR &= ~(1 << PIN_MOSI); // MOSI = 0
	PG_ODR &= ~(1 << PIN_CS);   // CS   = 0

	for (cnt = 0; cnt < 8; cnt++, cmd <<= 1)
	{
		if (cmd & 0x80)
		{
			PG_ODR |= (1 << PIN_MOSI);  // MOSI = 1
		}
		else
		{
			PG_ODR &= ~(1 << PIN_MOSI); // MOSI = 0
		}
		delay_us(1);
		PG_ODR |= (1 << PIN_SCK);  // SCK = 1
		delay_us(1);
		PG_ODR &= ~(1 << PIN_SCK); // SCK = 0
	}

	// busy time - skip one rise edge
	delay_us(1);
	PG_ODR |= (1 << PIN_SCK);   // SCK = 1
	delay_us(1);

	for (cnt = 0, data = 0; cnt < cnt_conv; cnt++)
	{
		// sequentially accept and shift the data bits - msb first
		data <<= 1;
		PG_ODR &= ~(1 << PIN_SCK); // SCK = 0
		delay_us(1);
		PG_ODR |= (1 << PIN_SCK);  // SCK = 1
		if ((PG_IDR & (1 << PIN_MISO)) >> PIN_MISO)
		{
			data += 1; 
		}
		delay_us(1);
	}

	PG_ODR |= (1 << PIN_CS);    // CS = 1
	delay_us(1);

	return data;
}

//--------------------------------------------
uint8_t hal_xpt2046_read_int(void)
{
	return ((PG_IDR & (1 << PIN_INT)) >> PIN_INT);
}
