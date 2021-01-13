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

#ifndef FATFS_DRV_H_
#define FATFS_DRV_H_

typedef struct fatfs_drv
{
	void (*init) (void);
	DRESULT (*reset) (void);
	DRESULT (*read) (uint8_t *rxbuf, uint32_t sector, uint32_t count);
	DRESULT (*write) (const uint8_t *txbuf, uint32_t sector, uint32_t count);
	DWORD (*getsectorcount) (void);
	WORD (*getsectorsize) (void);
	DWORD (*getblocksize) (void);
} fatfs_drv_t;

#endif // FATFS_DRV_H_
