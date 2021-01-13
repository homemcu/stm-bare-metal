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
#define	PIN_SCL         1   // PE1 --> SCK
#define	PIN_SDA         2   // PE2 <-> SDA

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
	// FREQ[5:0]: fMASTER = 16MHz
	I2C_FREQR = (1 << I2C_FREQR_FREQ4);
	// For instance: in standard mode, the maximum allowed SCL rise time is 1000 ns.
	// TRISE[5:0] = 16 + 1: fMASTER in MHz + 1
	I2C_TRISER = 17;
	// CCR = 80: fMASTER / 100kHz / 2
	I2C_CCRL = 80;
	// PE = 1: Peripheral enable
	I2C_CR1 = (1 << I2C_CR1_PE);
}

//--------------------------------------------
static uint8_t i2c_start(uint8_t addr)
{
	uint32_t cnt;

	I2C_CR2 |= (1 << I2C_CR2_START);
	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C_SR1 & (1 << I2C_SR1_SB)); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	I2C_DR = addr;
	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C_SR1 & (1 << I2C_SR1_ADDR)); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	I2C_SR3;
	return I2C_SUCCESS;
}

//--------------------------------------------
static inline void i2c_stop(void)
{
	I2C_CR2 |= (1 << I2C_CR2_STOP);
}

//--------------------------------------------
static uint8_t i2c_writebyte(uint8_t data)
{
	uint32_t cnt;

	I2C_DR = data;
	for (cnt = 0; (cnt < I2C_WAIT) && !(I2C_SR1 & (1 << I2C_SR1_TXE)); cnt++);
	if (cnt == I2C_WAIT)
	{
		return I2C_FAIL;
	}
	return I2C_SUCCESS;
}

//--------------------------------------------
static inline void i2c_slow(void)
{
	// 100kHz
	I2C_CCRL = 80;
}

//--------------------------------------------
static inline void i2c_fast(void)
{
	// 400kHz
	I2C_CCRL = 20;
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
