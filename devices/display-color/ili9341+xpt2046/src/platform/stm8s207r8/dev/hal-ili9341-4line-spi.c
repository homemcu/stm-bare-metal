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
#define	PIN_SCK         5   // PC5 --> SCK
#define	PIN_MOSI        6   // PC6 --> SDI(MOSI)
#define	PIN_RESET       5   // PG5 --> RESET
#define	PIN_CS          6   // PG6 --> CS
#define	PIN_DC          7   // PG7 --> DC

//--------------------------------------------
// SPI Data Transfer Frequency (10MHz max)
// SPI_CK = fMASTER(16MHz) / 2 = 8MHz
#define SPI_CLK_DIV     0

//--------------------------------------------
void hal_ili9341_init(void)
{
	// BDM = 1: 1-line bidirectional data mode selected
	// BDOE = 1: Output enabled (transmit-only mode)
	// SSM = 1: Software slave management enabled
	// SSI = 1: Internal slave select in master mode
	SPI_CR2 = (1 << SPI_CR2_BDM) | (1 << SPI_CR2_BDOE) | (1 << SPI_CR2_SSM) | (1 << SPI_CR2_SSI);
	// SPE = 1: SPI enable
	// BR[2:0]: Baud rate control
	// MSTR = 1: Master configuration
	SPI_CR1 = (1 << SPI_CR1_SPE) | SPI_CLK_DIV | (1 << SPI_CR1_MSTR);

	// Set first to prevent glitches
	PG_ODR |= (1 << PIN_CS);     // CS = 1
	PG_ODR |= (1 << PIN_RESET);  // RST = 1

	PG_DDR |= (1 << PIN_RESET) | (1 << PIN_CS) | (1 << PIN_DC);
	PG_CR1 |= (1 << PIN_RESET) | (1 << PIN_CS) | (1 << PIN_DC);
	PG_CR2 |= (1 << PIN_RESET) | (1 << PIN_CS) | (1 << PIN_DC);
}

//--------------------------------------------
void hal_ili9341_reset(void)
{
	PG_ODR &= ~(1 << PIN_RESET); // RST = 0
	delay_ms(1);
	PG_ODR |= (1 << PIN_RESET);  // RST = 1
	delay_ms(5);
}

//--------------------------------------------
void hal_ili9341_select(void)
{
	PG_ODR &= ~(1 << PIN_CS);    // CS = 0
}

//--------------------------------------------
void hal_ili9341_release(void)
{
	PG_ODR |= (1 << PIN_CS);     // CS = 1
}

//--------------------------------------------
void hal_ili9341_command(void)
{
	PG_ODR &= ~(1 << PIN_DC);    // DC = 0
}

//--------------------------------------------
void hal_ili9341_data(void)
{
	PG_ODR |= (1 << PIN_DC);     // DC = 1
}

//--------------------------------------------
void hal_ili9341_tx(uint8_t data)
{
	while (!(SPI_SR & (1 << SPI_SR_TXE)));
	SPI_DR = data;
}

//--------------------------------------------
void hal_ili9341_tx_complete(void)
{
	while ((SPI_SR & (1 << SPI_SR_BSY)));
}
