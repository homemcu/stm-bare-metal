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

#ifndef HAL_SD_SDIO_H_
#define HAL_SD_SDIO_H_

void hal_sd_sdio_init(void);
void hal_sd_sdio_init_dma(void);
sd_error hal_sd_sdio_reset(void);
sd_error hal_sd_sdio_read(uint8_t *rxbuf, uint32_t sector, uint32_t count);
sd_error hal_sd_sdio_write(const uint8_t *txbuf, uint32_t sector, uint32_t count);
sd_error hal_sd_sdio_read_dma(uint8_t *rxbuf, uint32_t sector, uint32_t count);
sd_error hal_sd_sdio_write_dma(const uint8_t *txbuf, uint32_t sector, uint32_t count);
sd_info_t* hal_sd_sdio_getcardinfo(void);

#endif // HAL_SD_SDIO_H_
