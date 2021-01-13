/*
* Copyright (c) 2019 Vladimir Alemasov
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

#ifndef HAL_SPI_I2S_H_
#define HAL_SPI_I2S_H_

void hal_spi_i2s_init(void);
void hal_spi_i2s_config(uint8_t master_transmit);
void hal_spi_i2s_reset(void);
double hal_spi_i2s_get_mclk(void);
void hal_spi_i2s_init_dma_tx_buf(void);
void hal_spi_i2s_init_dma_rx_buf(void);
void hal_spi_i2s_init_dma_tx_buf_cycle(void);
void hal_spi_i2s_init_dma_rx_buf_cycle(void);
void hal_spi_i2s_write_dma_buf(void *txbuf, uint32_t length);
void hal_spi_i2s_read_dma_buf(void *rxbuf, uint32_t length);
void hal_spi_i2s_write_dma_buf_stop(void *txbuf, uint32_t length, uint32_t timeout);
void hal_spi_i2s_stop_dma_tx(void);

#endif // HAL_SPI_I2S_H_
