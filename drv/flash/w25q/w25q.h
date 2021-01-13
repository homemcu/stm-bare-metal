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

#ifndef W25Q_SPI_H_
#define W25Q_SPI_H_

void w25q_init(void);
void w25q_read(uint8_t *buf, uint32_t addr, uint32_t size);
void w25q_writepage(uint8_t *buf, uint32_t addr, uint32_t size);
void w25q_erase(uint32_t addr, uint32_t size);
uint32_t w25q_getflashsize(void);
uint32_t w25q_getpagesize(void);
uint32_t w25q_geterasesectorsize(void);

#endif // W25Q_SPI_H_

