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

#ifndef ETH_UIP_DRV_H_
#define ETH_UIP_DRV_H_

typedef struct ethernet_uip_drv
{
	void (*init) (const uint8_t *mac);
	void (*send) (const uint8_t *buf, uint16_t buflen);
	uint16_t (*recv) (uint8_t *buf, uint16_t buflen);
} ethernet_uip_drv_t;

#endif // ETH_UIP_DRV_H_
