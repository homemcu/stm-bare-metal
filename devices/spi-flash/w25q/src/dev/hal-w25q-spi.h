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

#ifndef HAL_W25Q_SPI_H_
#define HAL_W25Q_SPI_H_

void hal_w25q_spi_init(void);
void hal_w25q_spi_select(void);
void hal_w25q_spi_release(void);
uint8_t hal_w25q_spi_txrx(uint8_t data);

#endif // HAL_W25Q_SPI_H_
