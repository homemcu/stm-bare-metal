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
#include "hal-int-rtc.h"
#include "rtc-drv.h"

//--------------------------------------------
const rtc_drv_t rtc_drv =
{
	hal_int_rtc_init,
	hal_int_rtc_settime,
	hal_int_rtc_gettime
};
