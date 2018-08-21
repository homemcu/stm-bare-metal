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
#include "spi-flash-drv.h"
#include <string.h>
#include <stdio.h>

#define SIZE   512

extern const spi_flash_drv_t flash_drv;

//--------------------------------------------
int main(void)
{
	static uint8_t buf1[SIZE];
	static uint8_t buf2[SIZE];
	uint32_t flash_size;
	uint32_t page_size;
	uint32_t erase_sector_size;
	uint8_t cnt;


	platform_init();

	flash_drv.init();
	flash_size = flash_drv.get_flash_size();
	page_size = flash_drv.get_page_size();
	erase_sector_size = flash_drv.get_erase_sector_size();
	printf("SPI Flash:\n\ttotal size = %lu bytes\n\tpage size = %lu bytes\n\terase sector size = %lu bytes\n\n",
	        flash_size, page_size, erase_sector_size);

	printf("Write and read %d bytes from 0 page...\n", SIZE);
	memset(buf1, 0xAA, SIZE);
	memset(buf2, 0, SIZE);
	flash_drv.erase(0, erase_sector_size);
	for (cnt = 0; cnt < SIZE / page_size; cnt++)
	{
		flash_drv.write_page(buf1, page_size * cnt, page_size);
	}
	flash_drv.read(buf2, 0, SIZE);
	if (!memcmp(buf1, buf2, SIZE))
	{
		printf("Data matched.\n");
	}
	else
	{
		printf("Failed: Read data differs from the data written.\n");
	}


	while(1);
	return 0;
}
