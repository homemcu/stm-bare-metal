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

#ifndef ENC28J60_H_
#define ENC28J60_H_

void enc28j60_init(const uint8_t *mac);
uint16_t enc28j60_recv_packet(uint8_t *buf, uint16_t buflen);
void enc28j60_send_packet(const uint8_t *buf, uint16_t buflen);

#endif // ENC28J60_H_
