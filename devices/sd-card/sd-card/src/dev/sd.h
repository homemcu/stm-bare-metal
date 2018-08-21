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

#ifndef SD_H_
#define SD_H_

typedef enum
{
	sd_err_ok = 0,
	sd_err_ctimeout,
	sd_err_ccrcfail,
	sd_err_dtimeout,
	sd_err_dcrcfail,
	sd_err_rxoverr,
	sd_err_txunderr,
	sd_err_stbiterr,
	sd_err_writefail,
	sd_err_unexpected_command,
	sd_err_not_supported,
	sd_err_wrong_status
} sd_error;

typedef enum
{
	CARD_SDSC,
	CARD_SDHC,
	CARD_SDXC
} sd_type;

typedef struct sd_info
{
	sd_type    type;            // type of the sd card
	uint32_t   rca;             // relative card address
	uint32_t   card_size;       // card size in sectors
	uint32_t   sect_size;       // sector size in bytes
	uint32_t   erase_size;      // erase block size in sectors
} sd_info_t;

#endif // SD_H_
