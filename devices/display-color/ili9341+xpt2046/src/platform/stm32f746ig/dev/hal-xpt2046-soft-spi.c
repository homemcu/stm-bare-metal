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
#define	PORT_SCK        GPIO_B // PB0  --> SCK(T_SCK)(T_CLK)
#define	PIN_SCK         0
#define	PORT_CS         GPIO_C // PC13 --> SCS(T_CS)
#define	PIN_CS          13
#define	PORT_MOSI       GPIO_F // PF11 --> SI(T_MOSI)(T_DIN)
#define	PIN_MOSI        11
#define	PORT_MISO       GPIO_B // PB2  <-- SO(T_MISO)(T_DO)
#define	PIN_MISO        2
#define	PORT_INT        GPIO_B // PB1  <-- INT(T_PEN)(T_IRQ)
#define	PIN_INT         1

//--------------------------------------------
void hal_xpt2046_init(void)
{
	// IO port B clock enable
	// IO port C clock enable
	// IO port F clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOFEN;

	hw_cfg_pin(GPIOx(PORT_SCK),   PIN_SCK,   GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_CS),    PIN_CS,    GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MOSI),  PIN_MOSI,  GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MISO),  PIN_MISO,  GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_INT),   PIN_INT,   GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);
}

//--------------------------------------------
uint16_t hal_xpt2046_read(uint8_t cmd)
{
	uint8_t cnt;
	uint8_t cnt_conv;
	uint16_t data;

	if (cmd & 0x08)
	{
		cnt_conv = 8;
	}
	else
	{
		cnt_conv = 12;
	}

	hw_set_pin(GPIOx(PORT_SCK), PIN_SCK, 0);
	hw_set_pin(GPIOx(PORT_MOSI), PIN_MOSI, 0);
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 0);

	for (cnt = 0; cnt < 8; cnt++, cmd <<= 1)
	{
		if (cmd & 0x80)
		{
			hw_set_pin(GPIOx(PORT_MOSI), PIN_MOSI, 1);
		}
		else
		{
			hw_set_pin(GPIOx(PORT_MOSI), PIN_MOSI, 0);
		}
		delay_us(1);
		hw_set_pin(GPIOx(PORT_SCK), PIN_SCK, 1);
		delay_us(1);
		hw_set_pin(GPIOx(PORT_SCK), PIN_SCK, 0);
	}

	// Busy time
	delay_us(1);
	hw_set_pin(GPIOx(PORT_SCK), PIN_SCK, 1);
	delay_us(1);

	for (cnt = 0, data = 0; cnt < cnt_conv; cnt++)
	{
		data <<= 1;
		hw_set_pin(GPIOx(PORT_SCK), PIN_SCK, 0);
		delay_us(1);
		hw_set_pin(GPIOx(PORT_SCK), PIN_SCK, 1);
		if (hw_get_pin(GPIOx(PORT_MISO), PIN_MISO))
		{
			data += 1; 
		}
		delay_us(1);
	}

	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);
	delay_us(1);

	return data;
}

//--------------------------------------------
uint8_t hal_xpt2046_read_int(void)
{
	return hw_get_pin(GPIOx(PORT_INT), PIN_INT);
}
