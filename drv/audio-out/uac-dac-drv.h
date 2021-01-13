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

#ifndef UAC_DAC_DRV_H_
#define UAC_DAC_DRV_H_

typedef void (*write_dma_txbuf_complete_callback)(void);
typedef void (*mclk_count_complete_callback)(uint32_t mclk);

typedef struct audio_out_drv
{
	void (*init)(void);
	void (*init_dma_txbuf)(write_dma_txbuf_complete_callback callback);
	void (*write_dma_txbuf)(void *txbuf, uint32_t length);
	double (*get_mclk)(void);
	void (*mclk_set_sof)(uint32_t sof);
	void (*mclk_start_count)(mclk_count_complete_callback callback);
	void (*mclk_stop_count)(void);
	void (*convert)(uint32_t *in, uint32_t *out);
} audio_out_drv_t;

#endif // UAC_DAC_DRV_H_
