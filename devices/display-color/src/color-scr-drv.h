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

#ifndef COLOR_SCR_DRV_H_
#define COLOR_SCR_DRV_H_

typedef struct color_scr_drv
{
	uint16_t (*width) (void);
	uint16_t (*height) (void);
	void (*init) (color_scr_mode_t mode);
	void (*set_orientation) (scr_orient_t orientation);
	void (*set_bound_rect) (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void (*start_memory_write) (void);
	void (*stop_memory_write) (void);
	void (*memory_write) (uint32_t color);
} color_scr_drv_t;

#endif // COLOR_SCR_DRV_H_
