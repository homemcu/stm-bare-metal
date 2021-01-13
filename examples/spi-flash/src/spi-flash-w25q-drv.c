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
#include "w25q.h"

//--------------------------------------------
const spi_flash_drv_t flash_drv =
{
	w25q_init,
	w25q_read,
	w25q_writepage,
	w25q_erase,
	w25q_getflashsize,
	w25q_getpagesize,
	w25q_geterasesectorsize
};
