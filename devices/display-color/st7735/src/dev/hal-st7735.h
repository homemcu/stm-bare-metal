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

#ifndef HAL_ST7735_H_
#define HAL_ST7735_H_

void hal_st7735_init(void);
void hal_st7735_select(void);
void hal_st7735_release(void);
void hal_st7735_command(void);
void hal_st7735_data(void);
void hal_st7735_tx(uint8_t data);

#endif // HAL_ST7735_H_
