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
#include "audio-out-drv.h"

static write_dma_txbuf_complete_callback tx_irq_tcif_callback;

//--------------------------------------------
static void init(void)
{
	hal_spi_i2s_init();
	hal_spi_i2s_config(1);
}

//--------------------------------------------
static void init_dma_txbuf(write_dma_txbuf_complete_callback callback)
{
	tx_irq_tcif_callback = callback;
	hal_spi_i2s_init_dma_tx_buf();
}

//--------------------------------------------
void hal_spi_i2s_tx_irq_tcif_callback(void)
{
	tx_irq_tcif_callback();
}

//--------------------------------------------
const audio_out_drv_t audio_out_drv =
{
	init,
	init_dma_txbuf,
	hal_spi_i2s_write_dma_buf,
};
