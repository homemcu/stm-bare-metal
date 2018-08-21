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

#ifndef SCR_H_
#define SCR_H_

typedef enum
{
	MONO_SCR_PSET	= 0,
	MONO_SCR_PRES	= 1
} mono_scr_color_t;

typedef enum
{
	RGB444 = 0,
	RGB565 = 1,
	RGB666 = 2
} color_scr_mode_t;

typedef enum
{
	SCR_ORIENT_0   = 0,
	SCR_ORIENT_90  = 1,
	SCR_ORIENT_180 = 2,
	SCR_ORIENT_270 = 3
} scr_orient_t;

#endif // SCR_H_
