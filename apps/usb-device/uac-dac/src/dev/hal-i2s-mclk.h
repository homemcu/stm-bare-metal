/*
* Copyright (c) 2019 Vladimir Alemasov
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

#ifndef HAL_I2S_MCLK_H_
#define HAL_I2S_MCLK_H_

void hal_i2s_mclk_init(void);
void hal_i2s_mclk_set_sof(uint32_t sof);
void hal_i2s_mclk_start_count(void);
void hal_i2s_mclk_stop_count(void);

#endif // HAL_I2S_MCLK_H_
