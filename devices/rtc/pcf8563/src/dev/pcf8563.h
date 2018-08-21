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

#ifndef PCF8563_H_
#define PCF8563_H_

uint8_t pcf8563_init(void);
void pcf8563_settime(const rtc_time_t *time);
void pcf8563_gettime(rtc_time_t *time);

#endif // PCF8563_H_
