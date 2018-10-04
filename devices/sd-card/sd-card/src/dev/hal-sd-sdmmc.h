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

#ifndef HAL_SD_SDMMC_H_
#define HAL_SD_SDMMC_H_

void hal_sd_sdmmc_init(void);
void hal_sd_sdmmc_init_dma(void);
sd_error hal_sd_sdmmc_reset(void);
sd_error hal_sd_sdmmc_read(uint8_t *rxbuf, uint32_t sector, uint32_t count);
sd_error hal_sd_sdmmc_write(const uint8_t *txbuf, uint32_t sector, uint32_t count);
sd_error hal_sd_sdmmc_read_dma(uint8_t *rxbuf, uint32_t sector, uint32_t count);
sd_error hal_sd_sdmmc_write_dma(const uint8_t *txbuf, uint32_t sector, uint32_t count);
sd_info_t* hal_sd_sdmmc_getcardinfo(void);

#endif // HAL_SD_SDMMC_H_
