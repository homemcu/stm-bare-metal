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
#include "pcf8563-regs.h"
#include "hal-pcf8563.h"

//--------------------------------------------
static uint8_t bin2bcd(uint8_t value)
{
	uint8_t res = 0;

	res = ((value / 10) << 4) | (value % 10);
	return res;
}

//--------------------------------------------
static uint8_t bcd2bin(uint8_t value)
{
	uint8_t res = 0;

	res = (value >> 4) * 10 + (value & 0x0F);
	return res;
}

//--------------------------------------------
uint8_t pcf8563_init(void)
{
	uint8_t buf[1];

	hal_pcf8563_init();
	hal_pcf8563_read_registers(PCF8563_VL_SECONDS, buf, sizeof(buf));
	if (buf[0] & PCF8563_VL_FLAG)
	{
		return RTC_TIME_NOT_VALID;
	}
	return RTC_TIME_VALID;
}

//--------------------------------------------
void pcf8563_settime(const rtc_time_t *time)
{
	uint8_t buf[7];

	buf[0] = bin2bcd(time->sec);
	buf[1] = bin2bcd(time->min);
	buf[2] = bin2bcd(time->hour);
	buf[3] = bin2bcd(time->date);
	buf[4] = bin2bcd(time->wday - 1);
	buf[5] = bin2bcd(time->month);
	buf[6] = bin2bcd(time->year);

	hal_pcf8563_write_registers(PCF8563_VL_SECONDS, buf, sizeof(buf));
}

//--------------------------------------------
void pcf8563_gettime(rtc_time_t *time)
{
	uint8_t buf[7];

	hal_pcf8563_read_registers(PCF8563_VL_SECONDS, buf, sizeof(buf));

	time->sec = bcd2bin(buf[0] & PCF8563_SECONDS_MASK);
	time->min = bcd2bin(buf[1] & PCF8563_MINUTES_MASK);
	time->hour = bcd2bin(buf[2] & PCF8563_HOURS_MASK);
	time->date = bcd2bin(buf[3] & PCF8563_DAYS_MASK);
	time->wday = bcd2bin((buf[4] & PCF8563_WEEKDAYS_MASK) + 1);
	time->month = bcd2bin(buf[5] & PCF8563_MONTHS_MASK);
	time->year = bcd2bin(buf[6]);
}
