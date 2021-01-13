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

#ifndef HAL_PCD8544_H_
#define HAL_PCD8544_H_

void hal_pcd8544_init(void);
void hal_pcd8544_select(void);
void hal_pcd8544_release(void);
void hal_pcd8544_command(void);
void hal_pcd8544_data(void);
void hal_pcd8544_tx(uint8_t data);

#endif // HAL_PCD8544_H_
