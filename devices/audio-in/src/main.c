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
#include "audio-in-drv.h"

//--------------------------------------------
extern const audio_in_drv_t audio_in_drv;

//--------------------------------------------
#define AUDIO_SAMPLE_RATE               I2S_FCLK
#define BITS_PER_AUDIO_SAMPLE           I2S_BITRES
#if BITS_PER_AUDIO_SAMPLE == 16
#define BYTES_PER_AUDIO_SAMPLE          2
#else
#define BYTES_PER_AUDIO_SAMPLE          4
#endif
#define AUDIO_CHANNELS                  1
#define SAMPLES_PER_AUDIO_FRAME         (AUDIO_SAMPLE_RATE / 1000)
#define BYTES_PER_AUDIO_FRAME           (SAMPLES_PER_AUDIO_FRAME * AUDIO_CHANNELS * BYTES_PER_AUDIO_SAMPLE)
#define AUDIO_FRAMES_IN_BUFFER          2

#define SOUND_SIZE                      60000

//--------------------------------------------
static uint8_t i2s_cnt;
static uint32_t total_i2s_cnt;
static uint32_t buff[AUDIO_FRAMES_IN_BUFFER][BYTES_PER_AUDIO_FRAME / 4];
static uint32_t sound[SOUND_SIZE];

//--------------------------------------------
static void rx_complete_callback(void);

//--------------------------------------------
void main(void)
{
	platform_init();
	audio_in_drv.init();
	audio_in_drv.init_dma_rxbuf(rx_complete_callback);
	audio_in_drv.read_dma_rxbuf(&buff[i2s_cnt][0], BYTES_PER_AUDIO_FRAME);
	while (1);
}

//--------------------------------------------
static void rx_complete_callback(void)
{
	static int stop;
	uint32_t *buf32;
	buf32 = (uint32_t *)&buff[i2s_cnt][0];
	++i2s_cnt;
	if (i2s_cnt == AUDIO_FRAMES_IN_BUFFER)
	{
		i2s_cnt = 0;
	}
	if (!stop)
	{
		audio_in_drv.read_dma_rxbuf(&buff[i2s_cnt][0], BYTES_PER_AUDIO_FRAME);
		for (uint32_t cnt = 0; cnt < BYTES_PER_AUDIO_FRAME / sizeof(uint32_t); cnt += 2)
		{
			audio_in_drv.convert(&buf32[cnt], &sound[total_i2s_cnt]);
			if (total_i2s_cnt == SOUND_SIZE)
			{
				stop = 1;
				return;
			}
			++total_i2s_cnt;
		}
	}
}
