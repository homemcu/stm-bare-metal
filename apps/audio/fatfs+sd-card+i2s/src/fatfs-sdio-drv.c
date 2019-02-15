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
#include "hal-sd-sdio.h"
#include <string.h>
#include "diskio.h"
#include "fatfs-drv.h"

#define SDIO_DMA_MODE   1


#if SDIO_DMA_MODE
#include "ffconf.h"            // FF_MAX_SS
//--------------------------------------------
static uint32_t buf_dma[FF_MAX_SS / 4];
#endif


//--------------------------------------------
static void init(void)
{
	hal_sd_sdio_init();
#if SDIO_DMA_MODE
	hal_sd_sdio_init_dma();
#endif
}

//--------------------------------------------
static DRESULT reset(void)
{
	sd_error err;
	err = hal_sd_sdio_reset();
	if (err == sd_err_ok)
	{
		return RES_OK;
	}
	else
	{
		return RES_ERROR;
	}
}

//--------------------------------------------
static DRESULT read(uint8_t *rxbuf, uint32_t sector, uint32_t count)
{
	sd_error err;

#if SDIO_DMA_MODE
	uint32_t cnt;

	for (cnt = 0; cnt < count; cnt++)
	{
		do
		{
			err = hal_sd_sdio_read_dma((uint8_t *)buf_dma, sector + cnt, 1);
		}
		// wait for Transfer State
		while (err == sd_err_wrong_status);
		if (err != sd_err_ok)
		{
			break;
		}
		memcpy(&rxbuf[cnt * FF_MAX_SS], buf_dma, FF_MAX_SS);
	}
#else
	err = hal_sd_sdio_read(rxbuf, sector, count);
#endif

	if (err == sd_err_ok)
	{
		return RES_OK;
	}
	else
	{
		return RES_ERROR;
	}
}

//--------------------------------------------
static DRESULT write(const uint8_t *txbuf, uint32_t sector, uint32_t count)
{
	sd_error err;

#if SDIO_DMA_MODE
	uint32_t cnt;

	for (cnt = 0; cnt < count; cnt++)
	{
		memcpy(buf_dma, &txbuf[cnt * FF_MAX_SS], FF_MAX_SS);
		do
		{
			err = hal_sd_sdio_write_dma((const uint8_t *)buf_dma, sector + cnt, 1);
		}
		// wait for Transfer State
		while (err == sd_err_wrong_status);
		if (err != sd_err_ok)
		{
			break;
		}
	}
#else
	err = hal_sd_sdio_write(txbuf, sector, count);
#endif

	if (err == sd_err_ok)
	{
		return RES_OK;
	}
	else
	{
		return RES_ERROR;
	}
}

//--------------------------------------------
static DWORD getsectorcount(void)
{
	return (DWORD)hal_sd_sdio_getcardinfo()->card_size;
}

//--------------------------------------------
static WORD getsectorsize(void)
{
	return (WORD)hal_sd_sdio_getcardinfo()->sect_size;
}

//--------------------------------------------
static DWORD getblocksize(void)
{
	return (DWORD)hal_sd_sdio_getcardinfo()->erase_size;
}

//--------------------------------------------
const fatfs_drv_t fat_drv =
{
	init,
	reset,
	read,
	write,
	getsectorcount,
	getsectorsize,
	getblocksize
};
