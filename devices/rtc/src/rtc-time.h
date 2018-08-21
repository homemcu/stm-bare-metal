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

#ifndef RTC_TIME_H_
#define RTC_TIME_H_

#define RTC_TIME_VALID         0
#define RTC_TIME_NOT_VALID     1

typedef struct rtc_time
{
	uint8_t year;    // 0 - 99
	uint8_t month;   // 1 - 12
	uint8_t date;    // 1 - 31
	uint8_t hour;    // 0 - 23
	uint8_t min;     // 0 - 59
	uint8_t sec;     // 0 - 59
	uint8_t wday;    // 1 - 7
} rtc_time_t;

#endif // RTC_TIME_H_