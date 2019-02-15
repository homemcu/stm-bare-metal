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

#ifndef UAC2_ADC_DRV_H_
#define UAC2_ADC_DRV_H_

typedef void (*read_dma_rxbuf_complete_callback)(void);

typedef struct audio_in_drv
{
	void (*init)(void);
	void (*config)(void);
	void (*init_dma_rxbuf)(read_dma_rxbuf_complete_callback callback);
	void (*read_dma_rxbuf)(void *rxbuf, uint32_t length);
	void (*convert)(uint32_t *in, uint32_t *out);
} audio_in_drv_t;

#endif // UAC2_ADC_DRV_H_
