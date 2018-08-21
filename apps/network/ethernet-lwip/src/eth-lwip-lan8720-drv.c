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
#include "mac.h"
#include "eth-lwip-drv.h"
#include "hal-lan8720-rmii.h"

//--------------------------------------------
const ethernet_lwip_drv_t eth_drv = {
									hal_lan8720_init,
									hal_lan8720_get_transmission_status,
									hal_lan8720_write_frame,
									hal_lan8720_start_transmission,
									hal_lan8720_get_reception_status,
									hal_lan8720_read_frame,
									hal_lan8720_release_reception_buffer
									};
