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

#ifndef OV7670_H_
#define OV7670_H_

void ov7670_init(uint8_t little_endian);
uint32_t ov7670_read_dma_buf(uint8_t *rxbuf, uint32_t length);

#endif // OV7670_H_
