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
#define	PIN_MOSI        6   // PC6 --> SI
#define	PIN_MISO        7   // PC7 <-- SO
#define	PIN_CS          3   // PD3 --> CS(0)
#define	PIN_RESET       2   // PD2 --> RST(0)

//--------------------------------------------
// SPI Data Transfer Frequency (20MHz max)
// SPI_CK = fMASTER(16MHz) / 2 = 8MHz
#define SPI_CLK_DIV     0

//--------------------------------------------
void hal_enc28j60_init(void)
{
	// BDM = 0: 2-line unidirectional data mode selected
	// BDOE = 0: irrelevant
	// SSM = 1: Software slave management enabled
	// SSI = 1: Internal slave select - master mode
	SPI_CR2 = (1 << SPI_CR2_SSM) | (1 << SPI_CR2_SSI);
	// SPE = 1: SPI enable
	// BR[2:0]: Baud rate control
	// MSTR = 1: Master configuration
	SPI_CR1 = (1 << SPI_CR1_SPE) | SPI_CLK_DIV | (1 << SPI_CR1_MSTR);

	PD_DDR |= (1 << PIN_RESET) | (1 << PIN_CS);
	PD_CR1 |= (1 << PIN_RESET) | (1 << PIN_CS);
	PD_CR2 |= (1 << PIN_RESET) | (1 << PIN_CS);

	PD_ODR |= (1 << PIN_CS);     // CS = 1
	PD_ODR |= (1 << PIN_RESET);  // RST = 1
	delay_ms(1);

	PD_ODR &= ~(1 << PIN_RESET); // RST = 0
	delay_ms(1);
	PD_ODR |= (1 << PIN_RESET);  // RST = 1
	delay_ms(2);
}

//--------------------------------------------
void hal_enc28j60_select(void)
{
	PD_ODR &= ~(1 << PIN_CS);    // CS = 0
}

//--------------------------------------------
void hal_enc28j60_release(void)
{
	PD_ODR |= (1 << PIN_CS);     // CS = 1
}

//--------------------------------------------
uint8_t hal_enc28j60_txrx(uint8_t data)
{
	while (!(SPI_SR & (1 << SPI_SR_TXE)));
	SPI_DR = data;
	while (!(SPI_SR & (1 << SPI_SR_RXNE)));
	return SPI_DR;
}
