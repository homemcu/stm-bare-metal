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
#include "stm32f4xx-hw.h"

//--------------------------------------------
// GPIO_AF5_SPI1(APB2):
// SPI1 SCK:   PA5    PB3
// SPI1 MISO:  PA6    PB4
// SPI1 MOSI:  PA7    PB5
// GPIO_AF5_SPI2(APB1):
// SPI2 SCK:   PB10   PB13    PI0
// SPI2 MISO:  PC2    PB14    PI2
// SPI2 MOSI:  PC3    PB15    PI3
// GPIO_AF6_SPI3(APB1):
// SPI3 SCK:   PB3    PC10
// SPI3 MISO:  PB4    PC11
// SPI3 MOSI:  PB5    PC12

//--------------------------------------------
#define	PORT_SCK        GPIO_A   // PA5 --> SCK
#define	PIN_SCK         5
#define	PORT_MISO       GPIO_A   // PA6 <-- SO
#define	PIN_MISO        6
#define	PORT_MOSI       GPIO_B   // PB5 --> SI
#define	PIN_MOSI        5
#define	PORT_RESET      GPIO_B   // PB0 --> RST(0)
#define	PIN_RESET       0
#define	PORT_CS         GPIO_B   // PB1 --> CS(0)
#define	PIN_CS          1

//--------------------------------------------
// SPI Data Transfer Frequency (20MHz max)
// SPI1_CK = PCLK2(84MHz) / 8 = 10.5MHz
#define SPI_CLK_DIV      SPI_CR1_BR_1

//--------------------------------------------
void hal_enc28j60_init(void)
{
	// SPI1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	// IO port A clock enable
	// IO port B clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

	hw_cfg_pin(GPIOx(PORT_SCK),    PIN_SCK,    GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MISO),   PIN_MISO,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MOSI),   PIN_MOSI,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_RESET),  PIN_RESET,  GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_CS),     PIN_CS,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	// BIDIMODE = 0: 2-line unidirectional data mode selected
	// BIDIOE = 0: irrelevant
	// SSM = 1: Software slave management enabled
	// SSI = 1: NSS pin is ignored
	// SPE = 1: SPI enable
	// BR[2:0]: Baud rate control
	// MSTR = 1: Master configuration
	SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CLK_DIV | SPI_CR1_MSTR;

	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);			// PIN_CS = 1
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);	// PIN_RESET = 1
	delay_ms(1);

	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 0);	// PIN_RESET = 0
	delay_ms(1);
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);	// PIN_RESET = 1
	delay_ms(2);
}

//--------------------------------------------
void hal_enc28j60_select(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 0);			// PIN_CS = 0
}

//--------------------------------------------
void hal_enc28j60_release(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);			// PIN_CS = 1
}

//--------------------------------------------
uint8_t hal_enc28j60_txrx(uint8_t data)
{
	while (!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = data;
	while (!(SPI1->SR & SPI_SR_RXNE));
	return SPI1->DR;
}
