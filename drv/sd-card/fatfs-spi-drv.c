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
#include "hal-sd-spi.h"
#include "sd.h"
#include "sd-spi.h"
#include "ff.h"
#include "diskio.h"
#include "fatfs-drv.h"

//--------------------------------------------
static DRESULT reset(void)
{
	sd_error err;

	err = sd_spi_reset();
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

	err = sd_spi_read(rxbuf, sector, count);
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

	err = sd_spi_write(txbuf, sector, count);
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
	sd_info_t *sdinfo;

	sdinfo = sd_spi_getcardinfo();
	return (DWORD)sdinfo->card_size;
}

//--------------------------------------------
static WORD getsectorsize(void)
{
	sd_info_t *sdinfo;

	sdinfo = sd_spi_getcardinfo();
	return (WORD)sdinfo->sect_size;
}

//--------------------------------------------
static DWORD getblocksize(void)
{
	sd_info_t *sdinfo;

	sdinfo = sd_spi_getcardinfo();
	return (DWORD)sdinfo->erase_size;
}

//--------------------------------------------
const fatfs_drv_t fat_drv =
{
	hal_sd_spi_init,
	reset,
	read,
	write,
	getsectorcount,
	getsectorsize,
	getblocksize
};
