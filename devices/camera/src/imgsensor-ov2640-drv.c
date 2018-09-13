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
#include "hal-imgsensor-dcmi.h"
#include "ov2640.h"
#include "imgsensor-drv.h"

//--------------------------------------------
const imgsensor_drv_t cam_drv =
{
	ov2640_init,
	hal_imgsensor_enable_clock,
	hal_imgsensor_disable_clock,
	hal_imgsensor_init_dma_cycle,
	hal_imgsensor_init_dma_buf,
	hal_imgsensor_start_dma_cycle,
	ov2640_read_dma_buf,
	hal_imgsensor_stop_dma,
	hal_imgsensor_wait_vsync,
	hal_imgsensor_wait_hsync,
	hal_imgsensor_read_fifo,
	hal_imgsensor_start_capture,
	hal_imgsensor_stop_capture
};
