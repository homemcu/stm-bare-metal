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

#ifndef SPI_FLASH_DRV_H_
#define SPI_FLASH_DRV_H_

typedef struct spi_flash_drv
{
	void (*init) (void);
	void (*read) (uint8_t *buf, uint32_t addr, uint32_t size);
	void (*write_page) (uint8_t *buf, uint32_t addr, uint32_t size);
	void (*erase) (uint32_t addr, uint32_t size);
	uint32_t (*get_flash_size) (void);
	uint32_t (*get_page_size) (void);
	uint32_t (*get_erase_sector_size) (void);
} spi_flash_drv_t;

#endif // SPI_FLASH_DRV_H_
