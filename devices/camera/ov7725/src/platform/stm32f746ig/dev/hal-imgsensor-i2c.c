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
// I2C1(APB1, SYSCLK, HSI)
// GPIO_AF4_I2C1
// SCL:   PB6    PB8
// SDA:   PB7    PB9
//
// I2C2(APB1, SYSCLK, HSI)
// GPIO_AF4_I2C2
// SCL:   PB10   PF1    PH4
// SDA:   PB11   PF0    PH5
//
// I2C3(APB1, SYSCLK, HSI)
// GPIO_AF4_I2C3
// SCL:   PA8    PH7
// SDA:   PC9    PH8
//
// I2C4(APB1, SYSCLK, HSI)
// GPIO_AF4_I2C4
// SCL:   PD12   PF14   PH11
// SDA:   PD13   PF15   PH12
//
// The first clock source in the brackets
// is selected as I2C clock by default,
// see RCC->DCKCFGR2 register

//--------------------------------------------
#define	PORT_SCL        GPIO_B   // PB8 --> SCL
#define	PIN_SCL         8
#define	PORT_SDA        GPIO_B   // PB9 <-> SDA
#define	PIN_SDA         9
#define	PORT_RESET      GPIO_B   // PB3 --> RESET
#define	PIN_RESET       3

//--------------------------------------------
#define I2C_SUCCESS         0
#define I2C_FAIL            1
#define I2C_WAIT            5000
#define I2C_ADDR_WRITE      0x42
#define I2C_ADDR_READ       0x43

//--------------------------------------------
void hal_imgsensor_init_i2c(void)
{
	// IO port B clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	// I2C1 clock enable
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	// Set first to prevent glitches
	hw_set_pin(GPIOx(PORT_SCL), PIN_SCL, 1);      // SCL = 1
	hw_set_pin(GPIOx(PORT_SDA), PIN_SDA, 1);      // SDA = 1
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);  // RST = 1

	hw_cfg_pin(GPIOx(PORT_SCL), PIN_SCL, GPIOCFG_MODE_ALT | GPIO_AF4_I2C1 | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_SDA), PIN_SDA, GPIOCFG_MODE_ALT | GPIO_AF4_I2C1 | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_RESET),  PIN_RESET,  GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_LOW  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);


	// I2C_TIMINGR:
	// fI2CCLK = fPCLK1(APB1) = 54Mhz
	// Analog filter on
	// Digital filter off
	// Standard-mode 100kHz
	// Rise time = 700 ns
	// Fall time = 100 ns
	// I2C_TIMINGR value has been calculated by STM32CubeMX
	I2C1->TIMINGR = 0x30C03444;

	// I2C_CR1:
	// PE = 1: Peripheral enable
	I2C1->CR1 |= I2C_CR1_PE;
}

//--------------------------------------------
void hal_imgsensor_hard_reset(void)
{
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 0);  // RST = 0
	delay_ms(5);
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);  // RST = 1
	delay_ms(10);
}

//--------------------------------------------
uint8_t hal_imgsensor_read_register(uint8_t reg, uint8_t *data)
{
	uint32_t cnt;

	I2C1->CR2 = I2C_ADDR_WRITE | (1 << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND | I2C_CR2_START;

	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->ISR & I2C_ISR_TXIS); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	I2C1->TXDR = reg;

	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->ISR & I2C_ISR_STOPF); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}

	I2C1->CR2 = I2C_ADDR_READ | (1 << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND | I2C_CR2_START | I2C_CR2_RD_WRN;

	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->ISR & I2C_ISR_RXNE); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	*data = I2C1->RXDR;

	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->ISR & I2C_ISR_STOPF); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}

	return I2C_SUCCESS;
}

//--------------------------------------------
uint8_t hal_imgsensor_write_register(uint8_t reg, uint8_t data)
{
	uint32_t cnt;

	I2C1->CR2 = I2C_ADDR_WRITE | (2 << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND | I2C_CR2_START;

	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->ISR & I2C_ISR_TXIS); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	I2C1->TXDR = reg;

	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->ISR & I2C_ISR_TXIS); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	I2C1->TXDR = data;

	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->ISR & I2C_ISR_STOPF); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}

	return I2C_SUCCESS;
}
