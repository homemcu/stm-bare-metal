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
#include "spiffs.h"
#include "spiffs-drv.h"
#include "w25q.h"

//--------------------------------------------
static s32_t read(u32_t addr, u32_t size, u8_t *dst)
{
	w25q_read(dst, addr, size);
	return SPIFFS_OK;
}

//--------------------------------------------
static s32_t write(u32_t addr, u32_t size, u8_t *src)
{
	w25q_writepage(src, addr, size);
	return SPIFFS_OK;
}

//--------------------------------------------
static s32_t erase(u32_t addr, u32_t size)
{
	w25q_erase(addr, size);
	return SPIFFS_OK;
}

//--------------------------------------------
const spiffs_drv_t spiffs_drv =
{
	w25q_init,
	read,
	write,
	erase
};
