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
#include "sd-card-drv.h"

//--------------------------------------------
const sd_card_drv_t sd_card_drv =
{
	hal_sd_spi_init,
	sd_spi_reset,
	sd_spi_read,
	sd_spi_write,
	sd_spi_getcardinfo
};
