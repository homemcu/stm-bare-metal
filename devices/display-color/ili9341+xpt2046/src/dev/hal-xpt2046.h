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

#ifndef HAL_XPT2046_H_
#define HAL_XPT2046_H_

void hal_xpt2046_init(void);
uint16_t hal_xpt2046_read(uint8_t cmd);
uint8_t hal_xpt2046_read_int(void);

#endif // HAL_XPT2046_H_
