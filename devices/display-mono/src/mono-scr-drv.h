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
#ifndef MONO_SCR_DRV_H_
#define MONO_SCR_DRV_H_

typedef struct mono_scr_drv
{
	const uint8_t width;
	const uint8_t height;
	uint8_t* (*buf) (void);
	void (*init) (void);
	void (*flush) (void);
} mono_scr_drv_t;

#endif // MONO_SCR_DRV_H_
