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
#include "ssd1306-cmds.h"
#include "ssd1306.h"
#include "mono-scr-drv.h"

//--------------------------------------------
const mono_scr_drv_t scr_drv =
{
	SSD1306_SCR_W,
	SSD1306_SCR_H,
	ssd1306_getbufaddr,
	ssd1306_init,
	ssd1306_flush
};
