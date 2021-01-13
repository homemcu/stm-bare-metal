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
// GPIO_AF4_I2C1:
// I2C1 SCL:   PB6    PB8
// I2C1 SDA:   PB7    PB9
// I2C1 SMBA:  PB5
// GPIO_AF4_I2C2:
// I2C2 SCL:   PB10   PF1    PH4
// I2C2 SDA:   PB11   PF0    PH5
// I2C2 SMBA:  PB12   PF2    PH6
// GPIO_AF4_I2C3:
// I2C3 SCL:   PA8    PH7
// I2C3 SDA:   PC9    PH8
// I2C3 SMBA:  PA9    PH9

//--------------------------------------------
#define	PORT_SCL        GPIO_B   // PB8 --> SCK
#define	PIN_SCL         8
#define	PORT_SDA        GPIO_B   // PB9 <-> SDA
#define	PIN_SDA         9

//--------------------------------------------
#define I2C_SUCCESS         0
#define I2C_FAIL            1
#define I2C_WAIT            50000
#define I2C_ADDR_WRITE      0x78
#define I2C_CTRLBYTE_CMD    0x00
#define I2C_CTRLBYTE_DATA   0x40

//--------------------------------------------
void hal_ssd1306_init(void)
{
	// IO port B clock enable
	// IO port G clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOGEN;
	// I2C1 clock enable
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	hw_cfg_pin(GPIOx(PORT_SCL),   PIN_SCL,   GPIOCFG_MODE_ALT | GPIO_AF4_I2C1 | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_SDA),   PIN_SDA,   GPIOCFG_MODE_ALT | GPIO_AF4_I2C1 | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_NONE);

	// I2C_CR2:
	// FREQ = 42: PCLK1 in MHz
	I2C1->CR2 = 42;

	// I2C_TRISE:
	// For instance: in Sm mode, the maximum allowed SCL rise time is 1000 ns
	// TRISE = 42 + 1: PCLK1 in MHz + 1
#if 0
	I2C1->TRISE = 43;
#else
	I2C1->TRISE = 16;
#endif

	// I2C_CCR:
	// CCR = 210: PCLK1 / 100kHz / 2
	I2C1->CCR = 210;

	// I2C_CR1:
	// PE = 1: Peripheral enable
	I2C1->CR1 |= I2C_CR1_PE;
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
static inline void i2c_slow(void)
{
	// I2C_CCR:
	// CCR = 210: PCLK1 / 100kHz / 2
	I2C1->CCR = 210;
}

//--------------------------------------------
static inline void i2c_fast(void)
{
	// I2C_CCR:
	// CCR = 53: PCLK1 / 400kHz / 2
	I2C1->CCR = 53;
}

//--------------------------------------------
void hal_ssd1306_command(uint8_t cmd)
{
	i2c_start(I2C_ADDR_WRITE);
	i2c_writebyte(I2C_CTRLBYTE_CMD);
	i2c_writebyte(cmd);
	i2c_stop();
}

//--------------------------------------------
void hal_ssd1306_startdata(void)
{
	i2c_fast();
	i2c_start(I2C_ADDR_WRITE);
	i2c_writebyte(I2C_CTRLBYTE_DATA);
}

//--------------------------------------------
void hal_ssd1306_data(uint8_t data)
{
	i2c_writebyte(data);
}

//--------------------------------------------
void hal_ssd1306_stopdata(void)
{
	i2c_stop();
	i2c_slow();
}
