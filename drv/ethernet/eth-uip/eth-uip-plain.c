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

/*
* Some of this code has been taken from the steel_ne article:
* http://we.easyelectronics.ru/electro-and-pc/stm32-uip-enc28j60.html
*/

#include "platform.h"
#include "eth-uip-drv.h"
#include "uip.h"
#include "uip_arp.h"
#include "timer.h"
#include "stdio.h"
#include "string.h"

static struct timer periodic_timer, arp_timer;
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
static struct uip_eth_addr mac = { { 0x42, 0x66, 0x00, 0x06, 0x92, 0x14 } };
extern const ethernet_uip_drv_t eth_drv;

//--------------------------------------------
static void eth_init(void)
{
	uip_ipaddr_t ipaddr;

	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);

	eth_drv.init(mac.addr);

	uip_init();
	uip_arp_init();

	uip_setethaddr(mac);
#ifdef UIP_EXAMPLE_DHCPC
	uip_ipaddr(ipaddr, 0,0,0,0);
#else
	uip_ipaddr(ipaddr, 192,168,1,100);
#endif
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192,168,1,1);
	uip_setdraddr(ipaddr);
	uip_ipaddr(ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);

#ifdef UIP_EXAMPLE_DHCPC
	dhcpc_init(&mac, 6);
#endif
#ifdef UIP_EXAMPLE_SMTP
	uip_ipaddr(ipaddr, 127,0,0,1);
	smtp_configure("localhost", ipaddr);
	SMTP_SEND("addressee@example.com", 0, "uip-testing@example.com",
				"Testing SMTP from uIP",
				"Test message sent by uIP\r\n");
#endif
#ifdef UIP_EXAMPLE_TELNETD
	telnetd_init();
#endif
#ifdef UIP_EXAMPLE_HELLO_WORLD
	hello_world_init();
#endif
#ifdef UIP_EXAMPLE_WEBSERVER
	httpd_init();
#endif
#if defined UIP_EXAMPLE_RESOLV || defined UIP_EXAMPLE_WEBCLIENT
	webclient_init();
	resolv_init();
	uip_ipaddr(ipaddr, 8,8,8,8);
	resolv_conf(ipaddr);
	resolv_query("homewsn.com");
#endif
}

//--------------------------------------------
void eth_loop(void)
{
	int cnt;

	eth_init();
	for (;;)
	{
		uip_len = eth_drv.recv((uint8_t *)uip_buf, UIP_BUFSIZE);
		if (uip_len > 0)
		{
			if (BUF->type == htons(UIP_ETHTYPE_IP)) 
			{
				uip_arp_ipin();
				uip_input();
				if (uip_len > 0)
				{
					uip_arp_out();
					eth_drv.send((uint8_t *)uip_buf, uip_len);
				}
			}
			else if (BUF->type == htons(UIP_ETHTYPE_ARP))
			{
				uip_arp_arpin();
				if (uip_len > 0)
				{
					eth_drv.send((uint8_t *)uip_buf, uip_len);
				}
			}
		}
		else if (timer_expired(&periodic_timer))
		{
			timer_reset(&periodic_timer);
			for (cnt = 0; cnt < UIP_CONNS; cnt++)
			{
				uip_periodic(cnt);
				if (uip_len > 0)
				{
					uip_arp_out();
					eth_drv.send((uint8_t *)uip_buf, uip_len);
				}
			}
#if UIP_UDP
			for (cnt = 0; cnt < UIP_UDP_CONNS; cnt++)
			{
				uip_udp_periodic(cnt);
				if (uip_len > 0)
				{
					uip_arp_out();
					eth_drv.send((uint8_t *)uip_buf, uip_len);
				}
			}
#endif
			if (timer_expired(&arp_timer))
			{
				timer_reset(&arp_timer);
				uip_arp_timer();
      		}
		}
		delay_ms(1);
	}
}

#if defined UIP_EXAMPLE_RESOLV || defined UIP_EXAMPLE_WEBCLIENT
void webclient_closed(void)
{
	printf("Webclient: connection closed\n");
}
void webclient_aborted(void)
{
	printf("Webclient: connection aborted\n");
}
void webclient_timedout(void)
{
	printf("Webclient: connection timed out\n");
}
void webclient_connected(void)
{
	printf("Webclient: connected, waiting for data...\n");
}
void webclient_datahandler(char *data, u16_t len)
{
	printf("Webclient: got %d bytes of data.\n", len);
}
void resolv_found(char *name, u16_t *ipaddr)
{
	if (ipaddr == 0)
	{
		printf("Host '%s' not found.\n", name);
	}
	else
	{
		printf("Found name '%s' = %d.%d.%d.%d\n", name,
				htons(ipaddr[0]) >> 8,
				htons(ipaddr[0]) & 0xff,
				htons(ipaddr[1]) >> 8,
				htons(ipaddr[1]) & 0xff);
#ifdef UIP_EXAMPLE_WEBCLIENT
		webclient_get("homewsn.com", 80, "/");
#endif
	}
}
#endif

#ifdef UIP_EXAMPLE_SMTP
void smtp_done(unsigned char error)
{
}
#endif
#ifdef UIP_EXAMPLE_DHCPC
void dhcpc_configured(const struct dhcpc_state *s)
{
	uip_sethostaddr(s->ipaddr);
	uip_setnetmask(s->netmask);
	uip_setdraddr(s->default_router);
	printf("Got IP address %d.%d.%d.%d\n",
			htons(s->ipaddr[0]) >> 8,
			htons(s->ipaddr[0]) & 0xff,
			htons(s->ipaddr[1]) >> 8,
			htons(s->ipaddr[1]) & 0xff);
#if 0
	resolv_conf(s->dnsaddr);
#endif
}
#endif
