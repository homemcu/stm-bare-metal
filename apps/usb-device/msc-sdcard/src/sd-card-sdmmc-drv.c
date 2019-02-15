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
#include "hal-sd-sdmmc.h"
#include "sd-card-drv.h"

//--------------------------------------------
const sd_card_drv_t sd_card_drv =
{
	hal_sd_sdmmc_init,
	hal_sd_sdmmc_reset,
	hal_sd_sdmmc_read,
	hal_sd_sdmmc_write,
	hal_sd_sdmmc_getcardinfo
};
