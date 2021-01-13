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
#include "xpt2046.h"
#include "touch-drv.h"

//--------------------------------------------
const touch_drv_t tch_drv =
{
	xpt2046_init,
	xpt2046_xpos,
	xpt2046_ypos,
	xpt2046_zpos
};
