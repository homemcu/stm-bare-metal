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
#include "rtc-time.h"
#include "pcf8563.h"
#include "rtc-drv.h"

//--------------------------------------------
const rtc_drv_t rtc_drv =
{
	pcf8563_init,
	pcf8563_settime,
	pcf8563_gettime
};
