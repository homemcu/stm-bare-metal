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

#ifndef HAL_PCF8563_H_
#define HAL_PCF8563_H_

void hal_pcf8563_init(void);
void hal_pcf8563_read_registers(uint8_t reg, uint8_t *data, uint8_t len);
void hal_pcf8563_write_registers(uint8_t reg, uint8_t *data, uint8_t len);

#endif // HAL_PCF8563_H_
