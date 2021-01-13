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
#include "audio-out-drv.h"
#include <string.h>
#include <math.h>

//--------------------------------------------
extern const audio_out_drv_t audio_out_drv;

//--------------------------------------------
#define AUDIO_SAMPLE_RATE               I2S_FCLK
#define BITS_PER_AUDIO_SAMPLE           I2S_BITRES
#if BITS_PER_AUDIO_SAMPLE == 16
#define BYTES_PER_AUDIO_SAMPLE          2
#else
#define BYTES_PER_AUDIO_SAMPLE          4
#endif
#define AUDIO_CHANNELS                  2
#define SAMPLES_PER_AUDIO_FRAME         (AUDIO_SAMPLE_RATE / 1000)
#define BYTES_PER_AUDIO_FRAME           (SAMPLES_PER_AUDIO_FRAME * AUDIO_CHANNELS * BYTES_PER_AUDIO_SAMPLE)
#define AUDIO_FRAMES_IN_BUFFER          2

//--------------------------------------------
#define SOUND_SIZE_32                  (AUDIO_SAMPLE_RATE * BYTES_PER_AUDIO_SAMPLE / sizeof(uint32_t))
#define SOUND_SIZE_16                  (AUDIO_SAMPLE_RATE * BYTES_PER_AUDIO_SAMPLE / sizeof(uint16_t))
#define PI                             3.14159265358979323846
#define TWO_PI                         (2.0 * PI)
#define FREQ_HZ                        2000.0

//--------------------------------------------
static uint8_t i2s_cnt;
static uint32_t total_i2s_cnt;
static uint32_t buff[AUDIO_FRAMES_IN_BUFFER][BYTES_PER_AUDIO_FRAME / 4];
static uint32_t sound[SOUND_SIZE_32];

//--------------------------------------------
static void tx_complete_callback(void);

//--------------------------------------------
void main(void)
{
	platform_init();
	audio_out_drv.init();
	audio_out_drv.init_dma_txbuf(tx_complete_callback);

#if BYTES_PER_AUDIO_SAMPLE == 2
	uint16_t *buf16 = (uint16_t *)sound;
	for (size_t cnt = 0; cnt < SOUND_SIZE_16; cnt += 2)
	{
		double t = ((double)cnt / 2.0) / SOUND_SIZE_16;
        buf16[cnt] = (int16_t)((double)INT16_MAX * sin(FREQ_HZ * TWO_PI * t)); // left
        buf16[cnt + 1] = buf16[cnt]; // right
    }
#elif BYTES_PER_AUDIO_SAMPLE == 4
	for (size_t cnt = 0; cnt < SOUND_SIZE_32; cnt += 2)
	{
		double t = ((double)cnt / 2.0) / SOUND_SIZE_32;
        sound[cnt] = (int32_t)((double)INT32_MAX * sin(FREQ_HZ * TWO_PI * t)); // left
        sound[cnt + 1] = sound[cnt]; // right
    }
#endif

	memcpy(&buff[i2s_cnt][0], &sound[0], BYTES_PER_AUDIO_FRAME);
	tx_complete_callback();
	while (1);
}

//--------------------------------------------
static void tx_complete_callback(void)
{
	audio_out_drv.write_dma_txbuf(&buff[i2s_cnt][0], BYTES_PER_AUDIO_FRAME);
	++i2s_cnt;
	if (i2s_cnt == AUDIO_FRAMES_IN_BUFFER)
	{
		i2s_cnt = 0;
	}
	total_i2s_cnt += BYTES_PER_AUDIO_FRAME;
	if (total_i2s_cnt == sizeof(sound))
	{
		total_i2s_cnt = 0;
	}
	memcpy(&buff[i2s_cnt][0], &sound[total_i2s_cnt / sizeof(uint32_t)], BYTES_PER_AUDIO_FRAME);
}
