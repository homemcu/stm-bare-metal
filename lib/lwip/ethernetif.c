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

/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/etharp.h"
#include "lwip/sys.h"
#include "lwip/snmp.h"
#include "ethernetif.h"

#include "mac.h"
#include "eth-lwip-drv.h"
extern const ethernet_lwip_drv_t eth_drv;
extern const uint8_t mac_addr[];

SemaphoreHandle_t s_xRxSemaphore = NULL;

#define INTERFACE_TASK_STACK_SIZE       (350)
#define INTERFACE_TASK_PRIORITY         (configMAX_PRIORITIES - 1)
#define TIME_WAITING_FOR_INPUT          (portMAX_DELAY)
/* Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
	/* set MAC hardware address length */
	netif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* set MAC hardware address */
	netif->hwaddr[0] = mac_addr[0];
	netif->hwaddr[1] = mac_addr[1];
	netif->hwaddr[2] = mac_addr[2];
	netif->hwaddr[3] = mac_addr[3];
	netif->hwaddr[4] = mac_addr[4];
	netif->hwaddr[5] = mac_addr[5];

	/* maximum transfer unit */
	netif->mtu = 1500;

	/* device capabilities */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

	s_xRxSemaphore = xSemaphoreCreateBinary();

	/* create the task that handles the ETH_MAC */
	xTaskCreate(ethernetif_input,
				"ethif",
				INTERFACE_TASK_STACK_SIZE,
				netif,
				INTERFACE_TASK_PRIORITY,
				(TaskHandle_t *) NULL);

	if (eth_drv.init(mac_addr) == mac_err_ok)
	{
		netif->flags |= NETIF_FLAG_LINK_UP;
	}

	/* Do whatever else is needed to initialize interface. */
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	struct pbuf *q;

	if (eth_drv.get_transmission_status() != mac_err_ok)
	{
		return ERR_TIMEOUT;
	}

#if ETH_PAD_SIZE
	pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

	for (q = p; q != NULL;)
	{
		/* Send the data from the pbuf to the interface, one pbuf at a
		time. The size of the data in each pbuf is kept in the ->len
		variable. */
		if (eth_drv.write_frame(q->payload, q->len) == mac_err_tx_need_start)
		{
			eth_drv.start_transmission();
		}
		else
		{
			q = q->next;
		}
	}
	eth_drv.start_transmission();

	MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
	if (((u8_t*)p->payload)[0] & 1)
	{
		/* broadcast or multicast packet*/
		MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
	}
	else
	{
		/* unicast packet */
		MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
	}
	/* increase ifoutdiscards or ifouterrors on error */

#if ETH_PAD_SIZE
	pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

	LINK_STATS_INC(link.xmit);

	return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input(struct netif *netif)
{
	struct pbuf *p, *q;
	u16_t len;
	u16_t size;

	/* Obtain the size of the packet and put it into the "len"
	variable. */
	if (eth_drv.get_reception_status(&size) != mac_err_ok)
	{
		return NULL;
	}

	len = size;

#if ETH_PAD_SIZE
	len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

	/* We allocate a pbuf chain of pbufs from the pool. */
	p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

	if (p != NULL)
	{

#if ETH_PAD_SIZE
		pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

		/* We iterate over the pbuf chain until we have read the entire
		* packet into the pbuf. */
		for (q = p; q != NULL; q = q->next)
		{
			/* Read enough bytes to fill this pbuf in the chain. The
			* available data in the pbuf is given by the q->len
			* variable.
			* This does not necessarily have to be a memcpy, you can also preallocate
			* pbufs for a DMA-enabled MAC and after receiving truncate it to the
			* actually received size. In this case, ensure the tot_len member of the
			* pbuf is the sum of the chained pbuf len members.
			*/
			eth_drv.read_frame(q->payload, q->len);
		}
		eth_drv.release_reception_buffer();

		MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
		if (((u8_t*)p->payload)[0] & 1)
		{
			/* broadcast or multicast packet*/
			MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
		}
		else
		{
			/* unicast packet*/
			MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
		}
#if ETH_PAD_SIZE
		pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

		LINK_STATS_INC(link.recv);
	}
	else
	{
		eth_drv.release_reception_buffer();
		LINK_STATS_INC(link.memerr);
		LINK_STATS_INC(link.drop);
		MIB2_STATS_NETIF_INC(netif, ifindiscards);
	}

	return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(void *pvParameters)
{
	struct pbuf *p;
	struct netif *netif = (struct netif *)pvParameters;

	for (;;)
	{
		if (xSemaphoreTake(s_xRxSemaphore, (TickType_t)TIME_WAITING_FOR_INPUT) == pdTRUE)
		{
			do
			{
				p = low_level_input(netif);
				if (p != NULL)
				{
					if (netif->input(p, netif) != ERR_OK)
					{
						pbuf_free(p);
					}
				}
			}
			while (p != NULL);
		}
	}
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));
  
#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;
	/* We directly use etharp_output() here to save a function call.
	* You can instead declare your own function an call etharp_output()
	* from it if you have to do some checks before sending (e.g. if link
	* is available...) */

#if LWIP_IPV4
#if LWIP_ARP || LWIP_ETHERNET
#if LWIP_ARP
	netif->output = etharp_output;
#else
	/* The user should write ist own code in low_level_output_arp_off function */
	netif->output = low_level_output_arp_off;
#endif /* LWIP_ARP */
#endif /* LWIP_ARP || LWIP_ETHERNET */
#endif /* LWIP_IPV4 */
 
#if LWIP_IPV6
	netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

	netif->linkoutput = low_level_output;

	/* initialize the hardware */
	low_level_init(netif);

	return ERR_OK;
}

//--------------------------------------------
void eth_irq_rx_callback(void)
{
	static portBASE_TYPE xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(s_xRxSemaphore, &xHigherPriorityTaskWoken);
	if (xHigherPriorityTaskWoken == pdTRUE)
	{
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
