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
#include "sd.h"
#include "sd-card-drv.h"
#include <string.h>
#include <stdio.h>

#define SIZE   512

extern const sd_card_drv_t sd_card_drv;

int main(void)
{
	static uint8_t buf1[SIZE];
	static uint8_t buf2[SIZE];
	sd_info_t *sdinfo;
	sd_error sderror;

	platform_init();

	sd_card_drv.init();
	sderror = sd_card_drv.reset();
	if (sderror != sd_err_ok)
	{
		printf("sd_card_drv.reset error %d.\n", sderror);
	}
	sdinfo = sd_card_drv.getcardinfo();
	switch(sdinfo->type)
	{
	case CARD_SDSC:
		printf("sdinfo:\n\ttype = SDSC\n");
		break;
	case CARD_SDHC:
		printf("sdinfo:\n\ttype = SDHC\n");
		break;
	case CARD_SDXC:
		printf("sdinfo:\n\ttype = SDXC\n");
		break;
	}
	printf("\trca = %#08lx\n", sdinfo->rca);
	printf("\tcard size = %lu sectors\n", sdinfo->card_size);
	printf("\tsector size = %lu bytes\n", sdinfo->sect_size);
	printf("\tminimum erase size = %lu sectors\n\n", sdinfo->erase_size);

//--------------------------------------------
// WARNING: The data on the SD-card will be lost!
//--------------------------------------------
#if 1
	printf("Write and read data in the 0 sector...\n");
	memset(buf1, 0xAA, SIZE);
	memset(buf2, 0, SIZE);
	sderror = sd_card_drv.write(buf1, 0, 1);
	if (sderror != sd_err_ok)
	{
		printf("sd_card_drv.write error %d.\n", sderror);
	}
	do
	{
		sderror = sd_card_drv.read(buf2, 0, 1);
	} while (sderror == sd_err_wrong_status);
	if (sderror != sd_err_ok)
	{
		printf("sd_card_drv.read error %d.\n", sderror);
	}
	if (!memcmp(buf1, buf2, SIZE))
	{
		printf("Data matched.\n");
	}
	else
	{
		printf("Failed: Read data differs from the data written.\n");
	}
#endif

	while(1);
	return 0;
}
