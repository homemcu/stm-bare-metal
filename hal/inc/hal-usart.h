/*
* Copyright (c) 2020 Vladimir Alemasov
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

#ifndef HAL_USART_H_
#define HAL_USART_H_

//--------------------------------------------
void hal_usart_init(void);
void hal_usart_config(struct usb_cdc_line_coding *cdc_line, bool hard_flow);
void hal_usart_init_dma_tx_buf(void);
void hal_usart_init_dma_rx_buf_cycle(void);
void hal_usart_write_dma_tx_buf(void *buf, uint32_t length);
void hal_usart_read_dma_rx_buf(void *buf, uint32_t length);
uint32_t hal_usart_get_dma_rx_count(void);
void hal_usart_stop_dma_tx(void);
void hal_usart_stop_dma_rx(void);

#endif // HAL_USART_H_


