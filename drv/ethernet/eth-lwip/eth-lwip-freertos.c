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

#include "lwip/tcpip.h"
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"

#include "FreeRTOS.h"
#include "task.h"

const uint8_t mac_addr[] = { 0x42, 0x66, 0x00, 0x06, 0x92, 0x14 };
struct netif xnetif;

//--------------------------------------------
static void eth_init(void)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	tcpip_init(NULL, NULL);

#if LWIP_DHCP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else
	IP4_ADDR(&ipaddr, 192,168,1,100);
	IP4_ADDR(&netmask, 255,255,255,0);
	IP4_ADDR(&gw, 192,168,1,1);
#endif

	netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
	netif_set_default(&xnetif);
	netif_set_up(&xnetif); 

#if LWIP_DHCP
	dhcp_start(&xnetif);
#endif
}

//--------------------------------------------------------------
void lwip_start(void *pvParameters)
{
	eth_init();
	for (;;)
	{
		vTaskSuspend(NULL);
	}
}

//--------------------------------------------
void eth_loop(void)
{
	xTaskCreate(lwip_start,
				"lwip",
				configMINIMAL_STACK_SIZE,
				NULL,
				tskIDLE_PRIORITY,
				(TaskHandle_t *) NULL);
	vTaskStartScheduler();
}
