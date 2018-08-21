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

#ifndef TOUCH_DRV_H_
#define TOUCH_DRV_H_

typedef struct touch_drv
{
	void (*init) (void);
	uint16_t (*xpos) (void);
	uint16_t (*ypos) (void);
	uint8_t (*zpos) (void);
} touch_drv_t;

#endif // TOUCH_DRV_H_
