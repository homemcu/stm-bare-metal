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

#ifndef IMGSENSOR_DRV_H_
#define IMGSENSOR_DRV_H_

typedef struct imgsensor_drv
{
	void (*init)(uint8_t little_endian);
	void (*enable_clock)(void);
	void (*disable_clock)(void);
	void (*init_dma_cycle)(void);
	void (*init_dma_buf)(void);
	void (*start_dma_cycle)(uint8_t *buf);
	uint32_t (*read_dma_buf)(uint8_t *buf, uint32_t length);
	void (*stop_dma)(void);
	void (*wait_vsync)(void);
	void (*wait_hsync)(void);
	uint32_t (*read_fifo)(void);
	void (*start_capture)(void);
	void (*stop_capture)(void);
} imgsensor_drv_t;

#endif // IMAGE_SENSOR_DRV_H_
