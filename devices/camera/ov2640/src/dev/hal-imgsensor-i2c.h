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

#ifndef HAL_OV7670_I2C_H_
#define HAL_OV7670_I2C_H_

#include "platform.h"

void hal_imgsensor_init_i2c(void);
void hal_imgsensor_hard_reset(void);
uint8_t hal_imgsensor_read_register(uint8_t reg, uint8_t *data);
uint8_t hal_imgsensor_write_register(uint8_t reg, uint8_t data);

#endif // HAL_OV7670_I2C_H_
