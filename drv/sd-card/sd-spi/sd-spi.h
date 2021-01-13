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

#ifndef SD_SPI_H_
#define SD_SPI_H_

void sd_spi_init(void);
sd_error sd_spi_reset(void);
sd_error sd_spi_read(uint8_t *rxbuf, uint32_t sector, uint32_t count);
sd_error sd_spi_write(const uint8_t *txbuf, uint32_t sector, uint32_t count);
sd_info_t* sd_spi_getcardinfo(void);

#endif // SD_SPI_H_

