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
#include "st7735.h"
#include "color-scr-drv.h"

//--------------------------------------------
const color_scr_drv_t scr_drv =
{
	st7735_getwidth,
	st7735_getheight,
	st7735_init,
	st7735_setorientation,
	st7735_setboundrect,
	st7735_startmemorywrite,
	st7735_stopmemorywrite,
	st7735_memorywrite
};
