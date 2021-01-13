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
// I2C1(APB1)
// GPIO_AF4_I2C1
// SCL:   PB6    PB8
// SDA:   PB7    PB9
//
// I2C2(APB1)
// GPIO_AF4_I2C2
// SCL:   PB10   PF1    PH4
// SDA:   PB11   PF0    PH5
//
// I2C3(APB1)
// GPIO_AF4_I2C3
// SCL:   PA8    PH7
// SDA:   PC9    PH8

//--------------------------------------------
#define	PORT_SCL        GPIO_B   // PB8 --> SCL
#define	PIN_SCL         8
#define	PORT_SDA        GPIO_B   // PB9 <-> SDA
#define	PIN_SDA         9
#define	PORT_RESET      GPIO_B   // PB0 --> RESET
#define	PIN_RESET       0

//--------------------------------------------
#define I2C_SUCCESS         0
#define I2C_FAIL            1
#define I2C_WAIT            5000
#define I2C_ADDR_WRITE      0x60
#define I2C_ADDR_READ       0x61

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

	hw_cfg_pin(GPIOx(PORT_SCL), PIN_SCL, GPIOCFG_MODE_ALT | GPIO_AF4_I2C1 | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_OPEN/*GPIOCFG_OTYPE_PUPD*/ | GPIOCFG_PUPD_NONE/*GPIOCFG_PUPD_PUP*/);
	hw_cfg_pin(GPIOx(PORT_SDA), PIN_SDA, GPIOCFG_MODE_ALT | GPIO_AF4_I2C1 | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_RESET),  PIN_RESET,  GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_LOW  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	// I2C_CR2:
	// FREQ = 42: fPCLK1(APB1) in MHz
	I2C1->CR2 = 42;

	// I2C_TRISE:
	// For instance: in Sm mode, the maximum allowed SCL rise time is 1000 ns
	// TRISE = 42 + 1: fPCLK1(APB1) in MHz + 1
	I2C1->TRISE = 16;

	// I2C_CCR:
	// CCR = 210: fPCLK1(APB1) / 100kHz / 2
	I2C1->CCR = 210;

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
static uint8_t i2c_start(uint8_t addr)
{
	uint32_t cnt;

	I2C1->CR1 |= I2C_CR1_START;
	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->SR1 & I2C_SR1_SB); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	I2C1->DR = addr;
	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->SR1 & I2C_SR1_ADDR); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	I2C1->SR2;
	return I2C_SUCCESS;
}

//--------------------------------------------
static inline void i2c_stop(void)
{
	I2C1->CR1 |= I2C_CR1_STOP;
}

//--------------------------------------------
static uint8_t i2c_writebyte(uint8_t data)
{
	uint32_t cnt;

	I2C1->DR = data;
	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->SR1 & I2C_SR1_TXE); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	return I2C_SUCCESS;
}

//--------------------------------------------
static inline void i2c_ack(void)
{
	I2C1->CR1 |= I2C_CR1_ACK;
}

//--------------------------------------------
static inline void i2c_nack(void)
{
	I2C1->CR1 &= ~I2C_CR1_ACK;
}

//--------------------------------------------
static uint8_t i2c_readbyte(uint8_t *data)
{
	uint32_t cnt;

	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C1->SR1 & I2C_SR1_RXNE); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	*data = I2C1->DR;
	return I2C_SUCCESS;
}

//--------------------------------------------
uint8_t hal_imgsensor_read_register(uint8_t reg, uint8_t *data)
{
	if (i2c_start(I2C_ADDR_WRITE) == I2C_FAIL)
	{
		return I2C_FAIL;
	}
	if (i2c_writebyte(reg) == I2C_FAIL)
	{
		return I2C_FAIL;
	}
	i2c_stop();
	if (i2c_start(I2C_ADDR_READ) == I2C_FAIL)
	{
		return I2C_FAIL;
	}
	i2c_nack();
	if (i2c_readbyte(data) == I2C_FAIL)
	{
		return I2C_FAIL;
	}
	i2c_stop();
	return I2C_SUCCESS;
}

//--------------------------------------------
uint8_t hal_imgsensor_write_register(uint8_t reg, uint8_t data)
{
	if (i2c_start(I2C_ADDR_WRITE) == I2C_FAIL)
	{
		return I2C_FAIL;
	}
	if (i2c_writebyte(reg) == I2C_FAIL)
	{
		return I2C_FAIL;
	}
	if (i2c_writebyte(data) == I2C_FAIL)
	{
		return I2C_FAIL;
	}
	i2c_stop();
	return I2C_SUCCESS;
}
