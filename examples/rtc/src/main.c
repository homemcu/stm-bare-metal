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
#include <stdio.h>
#include "rtc-time.h"
#include "rtc-drv.h"
#include "xprintf.h"

extern const rtc_drv_t rtc_drv;

//--------------------------------------------
int main(void)
{
	char line[100];
	long param;
	rtc_time_t time;
	uint8_t res;
	char *str = line;

	platform_init();

	res = rtc_drv.init();

	if (!res)
	{
		rtc_drv.gettime(&time);
		printf("Now: %d.%d.%d %d:%d:%d wday-%d\n", time.date, time.month, 2000 + time.year, time.hour, time.min, time.sec, time.wday);
		printf("Do you wanna setup a time? [Y/n]\n");
		// Do NOT use gets() in your regular application due to security reason.
		// Here it's only for compatibility with input/output via
		// IAR C-SPY Terminal I/O window and IAR/SDCC/GCC uart terminal
		// SDCC does not have scanf function
		gets(str);
		if (*str == 'Y' || *str == 'y')
			res = 1;
	}

	if (res == 1)
	{
		for (;;)
		{
			printf("Enter the current date/time in format <date> <month> <year> <hour> <min> <sec> <wday>:\n");
			// Do NOT use gets() in your regular application due to security reason.
			// Here it's only for compatibility with input/output via
			// IAR C-SPY Terminal I/O window and IAR/SDCC/GCC uart terminal
			// SDCC does not have scanf function
			gets(str);
			for (;;)
			{
				if (!(res = xatoi(&str, &param)))
				{
					break;
				}
				time.date = (uint8_t)param;
				if (!(res = xatoi(&str, &param)))
				{
					break;
				}
				time.month = (uint8_t)param;
				if (!(res = xatoi(&str, &param)))
				{
					break;
				}
				time.year = (uint8_t)(param - 2000);
				if (!(res = xatoi(&str, &param)))
				{
					break;
				}
				time.hour = (uint8_t)param;
				if (!(res = xatoi(&str, &param)))
				{
					break;
				}
				time.min = (uint8_t)param;
				if (!(res = xatoi(&str, &param)))
				{
					break;
				}
				time.sec = (uint8_t)param;
				if (!(res = xatoi(&str, &param)))
				{
					break;
				}
				time.wday = (uint8_t)param;
				break;
			}
			if (!res)
			{
				printf("The entered character string is invalid. Please try again.\n");
				continue;
			}
			rtc_drv.settime(&time);
			break;
		}
	}

	while (1)
	{
		rtc_drv.gettime(&time);
		printf("Now: %d.%d.%d %d:%d:%d wday-%d\n", time.date, time.month, 2000 + time.year, time.hour, time.min, time.sec, time.wday);
		delay_ms(5000);
	};

	return 0;
}
