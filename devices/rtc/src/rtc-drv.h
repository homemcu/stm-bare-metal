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

#ifndef RTC_DRV_H_
#define RTC_DRV_H_

typedef struct rtc_drv
{
	uint8_t (*init) (void);
	void (*settime) (const rtc_time_t *time);
	void (*gettime) (rtc_time_t *time);
} rtc_drv_t;

#endif // RTC_DRV_H_
