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
#include "stm32f7xx-hw.h"

//--------------------------------------------
// SPI1(APB2)
// GPIO_AF5_SPI1
// SCK:   PA5    PB3
// MISO:  PA6    PB4
// MOSI:  PA7    PB5
//
// SPI2(APB1)
// GPIO_AF5_SPI2
// SCK:   PA9    PB10   PB13   PD3    PI1
// MISO:  PB14   PC2    PI2
// MOSI:  PB15   PC1    PC3    PI3
//
// SPI3(APB1)
// GPIO_AF5_SPI3
// MOSI:  PD6
// GPIO_AF6_SPI3
// SCK:   PB3    PC10
// MISO:  PB4    PC11
// MOSI:  PB5    PC12
// GPIO_AF7_SPI3
// MOSI:  PB2
//
// SPI4(APB2)
// GPIO_AF5_SPI4
// SCK:   PE2    PE12
// MISO:  PE5    PE13
// MOSI:  PE6    PE14
//
// SPI5(APB2)
// GPIO_AF5_SPI5
// SCK:   PF7    PH6
// MISO:  PF8    PH7
// MOSI:  PF9    PF11
//
// SPI6(APB2)
// GPIO_AF5_SPI6
// SCK:   PG13
// MISO:  PG12
// MOSI:  PG14

//--------------------------------------------
#define	PORT_SCK        GPIO_I   // PI1  --> SCK
#define	PIN_SCK         1
#define	PORT_MOSI       GPIO_I   // PI3  --> SDI(MOSI)
#define	PIN_MOSI        3
#define	PORT_RESET      GPIO_H   // PH13 --> RESET
#define	PIN_RESET       13
#define	PORT_CS         GPIO_H   // PH14 --> CS
#define	PIN_CS          14
#define	PORT_DC         GPIO_H   // PH15 --> DC
#define	PIN_DC          15

//--------------------------------------------
// SPI Data Transfer Frequency (15MHz max)
// SPI2_CK = PCLK1(54MHz) / 4 = 13.5MHz
#define SPI_CLK_DIV      (SPI_CR1_BR_0)

//--------------------------------------------
void hal_st7735_init(void)
{
	// IO port I clock enable
	// IO port H clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN | RCC_AHB1ENR_GPIOHEN;
	// SPI2 clock enable
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

	// Set first to prevent glitches
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);	// PIN_RESET = 1
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);			// PIN_CS = 1
	hw_set_pin(GPIOx(PORT_DC), PIN_DC, 1);			// PIN_DC = 1

	hw_cfg_pin(GPIOx(PORT_SCK),    PIN_SCK,    GPIOCFG_MODE_ALT | GPIO_AF5_SPI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MOSI),   PIN_MOSI,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
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
	SPI2->CR1 = SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CLK_DIV | SPI_CR1_MSTR;
}

//--------------------------------------------
void hal_st7735_reset(void)
{
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 0);	// PIN_RESET = 0
	delay_ms(1);
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);	// PIN_RESET = 1
	delay_ms(2);
}

//--------------------------------------------
void hal_st7735_select(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 0);			// PIN_CS = 0
}

//--------------------------------------------
void hal_st7735_release(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);			// PIN_CS = 1
}

//--------------------------------------------
void hal_st7735_command(void)
{
	hw_set_pin(GPIOx(PORT_DC), PIN_DC, 0);			// PIN_DC = 0
}

//--------------------------------------------
void hal_st7735_data(void)
{
	hw_set_pin(GPIOx(PORT_DC), PIN_DC, 1);			// PIN_DC = 1
}

//--------------------------------------------
void hal_st7735_tx(uint8_t data)
{
	while (!(SPI2->SR & SPI_SR_TXE));
	*((volatile uint8_t *)&SPI2->DR) = data;
}

//--------------------------------------------
void hal_st7735_tx_complete(void)
{
	while ((SPI2->SR & SPI_SR_FTLVL));
	while ((SPI2->SR & SPI_SR_BSY));
}
