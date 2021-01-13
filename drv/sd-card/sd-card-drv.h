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

#ifndef SD_CARD_DRV_H_
#define SD_CARD_DRV_H_

typedef struct sd_card_drv
{
	void (*init) (void);
	sd_error (*reset) (void);
	sd_error (*read) (uint8_t *rxbuf, uint32_t sector, uint32_t count);
	sd_error (*write) (const uint8_t *txbuf, uint32_t sector, uint32_t count);
	sd_info_t* (*getcardinfo) (void);
} sd_card_drv_t;

#endif // SD_CARD_DRV_H_
