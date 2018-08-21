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

#ifndef SPIFFS_DRV_H_
#define SPIFFS_DRV_H_

typedef struct spiffs_drv
{
	void (*init) (void);
	s32_t (*read) (u32_t addr, u32_t size, u8_t *dst);
	s32_t (*write) (u32_t addr, u32_t size, u8_t *src);
	s32_t (*erase) (u32_t addr, u32_t size);
} spiffs_drv_t;

#endif // SPIFFS_DRV_H_

