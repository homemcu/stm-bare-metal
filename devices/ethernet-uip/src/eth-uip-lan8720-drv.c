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
#include "eth-uip-drv.h"
#include "mac.h"
#include "hal-lan8720-rmii.h"

static uint8_t rx_flag;

//--------------------------------------------
static void init(const uint8_t *mac)
{
	hal_lan8720_init(mac);
}

//--------------------------------------------
static void send_packet(const uint8_t *buf, uint16_t buflen)
{
	if (hal_lan8720_get_transmission_status() != mac_err_ok)
	{
		return;
	}
	hal_lan8720_write_frame(buf, buflen);
	hal_lan8720_start_transmission();
}

//--------------------------------------------
void eth_irq_rx_callback(void)
{
	rx_flag = 1;
}

//--------------------------------------------
static uint16_t recv_packet(uint8_t *buf, uint16_t buflen)
{
	uint16_t size;
	if (rx_flag)
	{
		rx_flag = 0;
		if (hal_lan8720_get_reception_status(&size) != mac_err_ok)
		{
			return 0;
		}
		size = hal_lan8720_read_frame(buf, buflen);
		hal_lan8720_release_reception_buffer();
		return size;
	}
	return 0;
}

//--------------------------------------------
const ethernet_uip_drv_t eth_drv =
{
	init,
	send_packet,
	recv_packet
};
