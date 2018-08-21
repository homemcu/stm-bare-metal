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

#ifndef ETH_LWIP_DRV_H_
#define ETH_LWIP_DRV_H_

typedef struct ethernet_lwip_drv
{
	mac_error (*init) (const uint8_t *mac);
	mac_error (*get_transmission_status) (void);
	mac_error (*write_frame) (const uint8_t *frame, uint16_t size);
	void (*start_transmission) (void);
	mac_error (*get_reception_status) (uint16_t *size);
	uint16_t (*read_frame) (uint8_t *frame, uint16_t size);
	void (*release_reception_buffer) (void);
} ethernet_lwip_drv_t;

#endif // ETH_LWIP_DRV_H_
