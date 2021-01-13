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
#include "enc28j60.h"
#include "eth-uip-drv.h"

//--------------------------------------------
const ethernet_uip_drv_t eth_drv =
{
	enc28j60_init,
	enc28j60_send_packet,
	enc28j60_recv_packet
};
