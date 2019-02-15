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

#include "platform.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ff.h"
#include "diskio.h"
#include "audio-out-drv.h"

//--------------------------------------------
#define BYTES_PER_AUDIO_FRAME           (512)
#define AUDIO_FRAMES_IN_BUFFER          4

//--------------------------------------------
extern const audio_out_drv_t audio_out_drv;

//--------------------------------------------
FATFS fatfs;
FIL file;
static bool read_next_chunk = false;
static uint8_t wav_cnt;
static uint8_t i2s_cnt;
static uint32_t total_i2s_cnt;
static uint32_t buff[AUDIO_FRAMES_IN_BUFFER][BYTES_PER_AUDIO_FRAME / 4];

//--------------------------------------------
void error(void)
{
    printf("Sorry! The test was failed.\n");
	while(1);
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
	read_next_chunk = true;
}

//--------------------------------------------
// Thanks to Aleksander Alekseev
// https://eax.me/stm32-i2s/
int playWavFile(const char* fname)
{
	printf("Openning %s...\r\n", fname);
	FIL file;
	FRESULT res = f_open(&file, fname, FA_READ);
	if(res != FR_OK) {
		printf("f_open() failed, res = %d\r\n", res);
		return -1;
	}

	printf("File opened, reading...\r\n");

	unsigned int bytesRead;
	uint8_t header[44];
	res = f_read(&file, header, sizeof(header), &bytesRead);
	if(res != FR_OK) {
		printf("f_read() failed, res = %d\r\n", res);
		f_close(&file);
		return -2;
	}

	if(memcmp((const char*)header, "RIFF", 4) != 0) {
		printf("Wrong WAV signature at offset 0: 0x%02X 0x%02X 0x%02X 0x%02X\r\n", header[0], header[1], header[2], header[3]);
		f_close(&file);
		return -3;
	}

	if(memcmp((const char*)header + 8, "WAVEfmt ", 8) != 0) {
		printf("Wrong WAV signature at offset 8!\r\n");
		f_close(&file);
		return -4;
	}

	if(memcmp((const char*)header + 36, "data", 4) != 0) {
		printf("Wrong WAV signature at offset 36!\r\n");
		f_close(&file);
		return -5;
	}

	uint32_t fileSize = 8 + (header[4] | (header[5] << 8) | (header[6] << 16) | (header[7] << 24));
	uint32_t headerSizeLeft = header[16] | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
	uint16_t compression = header[20] | (header[21] << 8);
	uint16_t channelsNum = header[22] | (header[23] << 8);
	uint32_t sampleRate = header[24] | (header[25] << 8) | (header[26] << 16) | (header[27] << 24);
	uint32_t bytesPerSecond = header[28] | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);
	uint16_t bytesPerSample = header[32] | (header[33] << 8);
	uint16_t bitsPerSamplePerChannel = header[34] | (header[35] << 8);
	uint32_t dataSize = header[40] | (header[41] << 8) | (header[42] << 16) | (header[43] << 24);

	printf(
		"--- WAV header ---\r\n"
		"File size: %lu\r\n"
		"Header size left: %lu\r\n"
		"Compression (1 = no compression): %d\r\n"
		"Channels num: %d\r\n"
		"Sample rate: %ld\r\n"
		"Bytes per second: %ld\r\n"
		"Bytes per sample: %d\r\n"
		"Bits per sample per channel: %d\r\n"
		"Data size: %ld\r\n"
		"------------------\r\n",
		fileSize, headerSizeLeft, compression, channelsNum, sampleRate, bytesPerSecond, bytesPerSample,
		bitsPerSamplePerChannel, dataSize);

	if(headerSizeLeft != 16) {
		printf("Wrong `headerSizeLeft` value, 16 expected\r\n");
		f_close(&file);
		return -6;
	}

	if(compression != 1) {
		printf("Wrong `compression` value, 1 expected\r\n");
		f_close(&file);
		return -7;
	}

	if(channelsNum != 2) {
		printf("Wrong `channelsNum` value, 2 expected\r\n");
		f_close(&file);
		return -8;
	}

	if((sampleRate != 44100) || (bytesPerSample != 4) || (bitsPerSamplePerChannel != 16) || (bytesPerSecond != 44100*2*2)
	   || (dataSize < AUDIO_FRAMES_IN_BUFFER * BYTES_PER_AUDIO_FRAME)) {
		printf("Wrong file format, 16 bit file with sample rate 44100 expected\r\n");
		f_close(&file);
		return -9;
	}

	for (i2s_cnt = 0; i2s_cnt < AUDIO_FRAMES_IN_BUFFER; i2s_cnt++)
	{
		res = f_read(&file, (uint8_t*)buff[i2s_cnt], BYTES_PER_AUDIO_FRAME, &bytesRead);
		if(res != FR_OK) {
			printf("f_read() failed, res = %d\r\n", res);
			f_close(&file);
			return -10;
		}
	}

	wav_cnt = 0;
	i2s_cnt = 0;
	total_i2s_cnt = 0;

	tx_complete_callback();
	read_next_chunk = false;

	while(dataSize >= BYTES_PER_AUDIO_FRAME) {
		if(!read_next_chunk) {
			continue;
		}

		read_next_chunk = false;

		res = f_read(&file, (uint8_t*)buff[wav_cnt], BYTES_PER_AUDIO_FRAME, &bytesRead);
		if(res != FR_OK) {
			printf("f_read() failed, res = %d\r\n", res);
			f_close(&file);
			return -13;
		}
		dataSize -= BYTES_PER_AUDIO_FRAME;

		++wav_cnt;
		if (wav_cnt == AUDIO_FRAMES_IN_BUFFER)
		{
			wav_cnt = 0;
		}

	}

	res = f_close(&file);
	if(res != FR_OK) {
		printf("f_close() failed, res = %d\r\n", res);
		return -14;
	}

	return 0;
}

//--------------------------------------------
void main(void)
{
	int res;

	platform_init();
	audio_out_drv.init();
	audio_out_drv.init_dma_txbuf(tx_complete_callback);

	if (f_mount(&fatfs, "", 0) != FR_OK)
	{
		error();
	}
	else
	{
	    printf("The SD card was successfully mounted.\n");
	}

	res = playWavFile("homeward.wav");

	if (res)
	{
		error();
	}
	else
	{
	    printf("Congratulations! The test was passed.\n");
	}

	while (1);
}
