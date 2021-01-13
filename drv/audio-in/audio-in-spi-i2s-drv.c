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

#include "platform.h"
#include "hal-spi-i2s.h"
#include "audio-in-drv.h"

static read_dma_rxbuf_complete_callback rx_irq_tcif_callback;

//--------------------------------------------
static void init(void)
{
	hal_spi_i2s_init();
	hal_spi_i2s_config(0);
}

//--------------------------------------------
static void init_dma_rxbuf(read_dma_rxbuf_complete_callback callback)
{
	rx_irq_tcif_callback = callback;
	hal_spi_i2s_init_dma_rx_buf();
}

//--------------------------------------------
void hal_spi_i2s_rx_irq_tcif_callback(void)
{
	rx_irq_tcif_callback();
}

//--------------------------------------------
// Byte sequence in I2S wire (32-bit, MSB first):            B1 B2 B3 B4
// Byte sequence after SPI-I2S conversion and DMA transfer:  B2 B1 B4 B3
// Byte sequence after this function (32-bit, LSB first):    B4 B3 B2 B1
static void convert32to32(uint32_t *in, uint32_t *out)
{
	*out = *in << 16 || *in >> 16;
}

//--------------------------------------------
const audio_in_drv_t audio_in_drv =
{
	init,
	init_dma_rxbuf,
	hal_spi_i2s_read_dma_buf,
	convert32to32,
};
