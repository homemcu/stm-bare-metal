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
#include "hal-i2s-mclk.h"
#include "uac-dac-drv.h"

static write_dma_txbuf_complete_callback tx_irq_tcif_callback;
static mclk_count_complete_callback mclk_callback;

//--------------------------------------------
static void init_dma_txbuf(write_dma_txbuf_complete_callback callback)
{
	tx_irq_tcif_callback = callback;
	hal_spi_i2s_init_dma_tx_buf();
}

//--------------------------------------------
static void init(void)
{
	hal_spi_i2s_init();
	hal_spi_i2s_config(1);
	hal_i2s_mclk_init();
}

//--------------------------------------------
void hal_spi_i2s_tx_irq_tcif_callback(void)
{
	tx_irq_tcif_callback();
}

//--------------------------------------------
static void mclk_start_count(mclk_count_complete_callback callback)
{
	mclk_callback = callback;
	hal_i2s_mclk_start_count();
}

//--------------------------------------------
void mclk_irq_callback(uint32_t mclk)
{
	mclk_callback(mclk);
}

//--------------------------------------------
// Byte sequence in USB wire (32-bit, LSB first):            B1 B2 B3 B4
// Byte sequence after this function before DMA transfer:    B3 B4 B1 B2
// Byte sequence in I2S wire (32-bit, MSB first):            B4 B3 B2 B1
static void convert32to32(uint32_t *in, uint32_t *out)
{
	*out = *in << 16 | *in >> 16;
}

//--------------------------------------------
const audio_out_drv_t audio_out_drv =
{
	init,
	init_dma_txbuf,
	hal_spi_i2s_write_dma_buf,
	hal_spi_i2s_get_mclk,
	hal_i2s_mclk_set_sof,
	mclk_start_count,
	hal_i2s_mclk_stop_count,
	convert32to32,
};
