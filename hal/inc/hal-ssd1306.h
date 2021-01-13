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

#ifndef HAL_SSD1306_H_
#define HAL_SSD1306_H_

#if !defined SSD1306_128_64 && !defined SSD1306_128_32
#error "At least one SSD1306 display must be defined: SSD1306_128_64 or SSD1306_128_32"
#endif

void hal_ssd1306_init(void);
void hal_ssd1306_command(uint8_t cmd);
void hal_ssd1306_startdata(void);
void hal_ssd1306_data(uint8_t data);
void hal_ssd1306_stopdata(void);

#endif // HAL_SSD1306_H_
