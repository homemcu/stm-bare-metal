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
#define	PORT_SCK        GPIO_A   // PA5 --> CLK
#define	PIN_SCK         5
#define	PORT_MOSI       GPIO_B   // PB5 --> DIN
#define	PIN_MOSI        5
#define	PORT_RESET      GPIO_E   // PE2 --> RST
#define	PIN_RESET       2
#define	PORT_CS         GPIO_E   // PE3 --> CE
#define	PIN_CS          3
#define	PORT_DC         GPIO_E   // PE4 --> DC
#define	PIN_DC          4

//--------------------------------------------
// SPI Data Transfer Frequency (4MHz max)
// SPI1_CK = PCLK2(84MHz) / 32 = 2.625MHz
#define SPI_CLK_DIV      SPI_CR1_BR_2

//--------------------------------------------
void hal_pcd8544_init(void)
{
	// IO port A clock enable
	// IO port B clock enable
	// IO port E clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOEEN;
	// SPI1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	hw_cfg_pin(GPIOx(PORT_SCK),    PIN_SCK,    GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MOSI),   PIN_MOSI,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_RESET),  PIN_RESET,  GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_CS),     PIN_CS,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_DC),     PIN_DC,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	// BIDIMODE = 1: 1-line bidirectional data mode selected
	// BIDIOE = 1: Output enabled (transmit-only mode)
	// SSM = 1: Software slave management enabled
	// SSI = 1: Internal slave select in master mode
	// SPE = 1: SPI enable
	// BR[2:0]: Baud rate control
	// MSTR = 1: Master configuration
	SPI1->CR1 = SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CLK_DIV | SPI_CR1_MSTR;

	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);        // CS = 1
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);  // RST = 1
	delay_ms(1);

	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 0);  // RST = 0
	delay_ms(1);
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);  // RST = 1
	delay_ms(2);
}

//--------------------------------------------
void hal_pcd8544_select(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 0);        // PIN_CS = 0
}

//--------------------------------------------
void hal_pcd8544_release(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);        // PIN_CS = 1
}

//--------------------------------------------
void hal_pcd8544_command(void)
{
	hw_set_pin(GPIOx(PORT_DC), PIN_DC, 0);        // PIN_DC = 0
}

//--------------------------------------------
void hal_pcd8544_data(void)
{
	hw_set_pin(GPIOx(PORT_DC), PIN_DC, 1);        // PIN_DC = 1
}

//--------------------------------------------
void hal_pcd8544_tx(uint8_t data)
{
	SPI1->DR = data;
	while (!(SPI1->SR & SPI_SR_TXE));
#if 0
	while ((SPI1->SR & SPI_SR_BSY));
#endif
}
