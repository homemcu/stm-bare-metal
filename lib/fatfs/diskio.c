/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

/*
* Copyright (c) 2018, 2021 Vladimir Alemasov
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

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */


#include "platform.h"
#include "fatfs-drv.h"


static volatile
DSTATUS Stat = STA_NOINIT;	/* Physical drive status */

extern const fatfs_drv_t fat_drv;


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
  	if (pdrv)
	{
		return STA_NOINIT;			/* Supports only drive 0 */
	}
	return Stat;
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	if (pdrv)
	{
		return STA_NOINIT;			/* Supports only drive 0 */
	}

	fat_drv.init();
	if (fat_drv.reset() != RES_OK)
	{
		Stat |= STA_NOINIT;
	}
	else
	{
		Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
	}

	return Stat;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	if (pdrv)
	{
		return RES_PARERR;			/* Supports only drive 0 */
	}
	if (Stat & STA_NOINIT)
	{
		return RES_NOTRDY;
	}

	return fat_drv.read(buff, sector, count);
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	if (pdrv)
	{
		return RES_PARERR;			/* Supports only drive 0 */
	}
	if (Stat & STA_NOINIT)
	{
		return RES_NOTRDY;
	}

	return fat_drv.write(buff, sector, count);
}


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_ERROR;

	if (pdrv)
	{
		return RES_PARERR;			/* Supports only drive 0 */
	}
	if (Stat & STA_NOINIT)
	{
		return RES_NOTRDY;
	}

	switch (cmd)
	{
	/* Complete pending write process (needed at FF_FS_READONLY == 0) */
	case CTRL_SYNC :
		res = RES_OK;
		break;

	/* Get media size in sectors (needed at FF_USE_MKFS == 1) */
	case GET_SECTOR_COUNT:
		*(DWORD*)buff = fat_drv.getsectorcount();
		res = RES_OK;
		break;

	/* Get sector size in bytes (needed at FF_MAX_SS != FF_MIN_SS) */
	case GET_SECTOR_SIZE:
		*(WORD*)buff = fat_drv.getsectorsize();
		res = RES_OK;
		break;

	/* Get erase block size in sectors (needed at FF_USE_MKFS == 1) */
	case GET_BLOCK_SIZE:
		*(DWORD*)buff = fat_drv.getblocksize();
		res = RES_OK;
		break;

	/* Inform device that the data on the block of sectors is no longer used (needed at FF_USE_TRIM == 1) */
	case CTRL_TRIM:
	default:
		res = RES_PARERR;
	}

	return res;
}

/*---------------------------------------------------------*/
/* User provided RTC function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called back     */
/* from FatFs module.                                      */

#if !FF_FS_NORTC && !FF_FS_READONLY
DWORD get_fattime (void)
{
	return 0;
}
#endif
