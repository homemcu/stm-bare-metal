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
#define	PIN_SCK         5   // PC5 --> CLK/SCLK
#define	PIN_MOSI        6   // PC6 --> CMD/DI
#define	PIN_MISO        7   // PC7 <-- DAT0/DO
#define	PIN_CS          3   // PC3 --> CD/DAT3/CS

//--------------------------------------------
// SPI Initialization Frequency (400kHz max)
// SPI1_CK = fMASTER/64 = 385kHz
#define SPI_INIT_CLK_DIV          (1 << SPI_CR1_BR2) | (1 << SPI_CR1_BR0)
// SPI Data Transfer Frequency (25MHz max)
// SPI1_CK = fMASTER/2 = 8MHz
#define SPI_TRANSFER_CLK_DIV      0

//--------------------------------------------
void hal_sd_spi_init(void)
{
	// BDM = 0: 2-line unidirectional data mode selected
	// BDOE = 0: irrelevant
	// SSM = 1: Software slave management enabled
	// SSI = 1: Internal slave select - master mode
	SPI_CR2 = (1 << SPI_CR2_SSM) | (1 << SPI_CR2_SSI);

	PC_DDR |= (1 << PIN_CS);
	PC_CR1 |= (1 << PIN_CS);
	PC_CR2 |= (1 << PIN_CS);

	PC_ODR |= (1 << PIN_CS);     // CS = 1
}

//--------------------------------------------
void hal_sd_spi_select(void)
{
	PC_ODR &= ~(1 << PIN_CS);    // CS = 0
}

//--------------------------------------------
void hal_sd_spi_release(void)
{
	PC_ODR |= (1 << PIN_CS);     // CS = 1
}

//--------------------------------------------
void hal_sd_spi_slow(void)
{
	// SPE = 1: SPI enable
	// BR[2:0] = SPI_INIT_CLK_DIV
	// MSTR = 1: Master configuration
	SPI_CR1 = (1 << SPI_CR1_SPE) | SPI_INIT_CLK_DIV | (1 << SPI_CR1_MSTR);
}

//--------------------------------------------
void hal_sd_spi_fast(void)
{
	// SPE = 1: SPI enable
	// BR[2:0] = SPI_TRANSFER_CLK_DIV
	// MSTR = 1: Master configuration
	SPI_CR1 = (1 << SPI_CR1_SPE) | SPI_TRANSFER_CLK_DIV | (1 << SPI_CR1_MSTR);
}

//--------------------------------------------
uint8_t hal_sd_spi_txrx(uint8_t data)
{
	while (!(SPI_SR & (1 << SPI_SR_TXE)));
	SPI_DR = data;
	while (!(SPI_SR & (1 << SPI_SR_RXNE)));
	return SPI_DR;
}
