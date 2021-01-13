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
#include "scr.h"
#include "ili9341.h"
#include "color-scr-drv.h"

//--------------------------------------------
const color_scr_drv_t scr_drv =
{
	ili9341_getwidth,
	ili9341_getheight,
	ili9341_init,
	ili9341_setorientation,
	ili9341_setboundrect,
	ili9341_startmemorywrite,
	ili9341_stopmemorywrite,
	ili9341_memorywrite
};
