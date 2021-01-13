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

#ifndef HAL_INT_RTC_H_
#define HAL_INT_RTC_H_

uint8_t hal_int_rtc_init(void);
void hal_int_rtc_settime(const rtc_time_t *time);
void hal_int_rtc_gettime(rtc_time_t *time);

#endif // HAL_INT_RTC_H_