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

#ifndef HAL_ENC28J60_H_
#define HAL_ENC28J60_H_

void hal_enc28j60_init(void);
void hal_enc28j60_select(void);
void hal_enc28j60_release(void);
uint8_t hal_enc28j60_txrx(uint8_t data);
#define hal_enc28j60_rx() hal_enc28j60_txrx(0xFF)
#define hal_enc28j60_tx(data) hal_enc28j60_txrx(data)

#endif // HAL_ENC28J60_H_
