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

#ifndef HAL_OV7670_DCMI_H_
#define HAL_OV7670_DCMI_H_

#include "platform.h"

void hal_imgsensor_init_capture(void);
void hal_imgsensor_init_clock(void);
void hal_imgsensor_enable_clock(void);
void hal_imgsensor_disable_clock(void);
void hal_imgsensor_init_dma_cycle(void);
void hal_imgsensor_init_dma_buf(void);
void hal_imgsensor_start_dma_cycle(uint8_t *buf);
uint32_t hal_imgsensor_read_dma_buf(uint8_t *buf, uint32_t length, uint32_t timeout);
void hal_imgsensor_stop_dma(void);
void hal_imgsensor_wait_vsync(void);
void hal_imgsensor_wait_hsync(void);
uint32_t hal_imgsensor_read_fifo(void);
void hal_imgsensor_start_capture(void);
void hal_imgsensor_stop_capture(void);

#endif // HAL_OV7670_DCMI_H_
