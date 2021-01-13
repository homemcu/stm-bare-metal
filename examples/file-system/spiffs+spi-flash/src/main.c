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
#include "spiffs.h"
#include "spiffs-drv.h"

#define LOG_PAGE_SIZE       256

extern const spiffs_drv_t spiffs_drv;

static spiffs fs;
static uint8_t spiffs_work_buf[LOG_PAGE_SIZE * 2];
static uint8_t spiffs_fds[32 * 4];
static uint8_t spiffs_cache_buf[(LOG_PAGE_SIZE + 32) * 4];

//--------------------------------------------
void spiffs_mount()
{
	spiffs_config cfg;
	int res;

#if !SPIFFS_SINGLETON
	cfg.phys_size = 2UL * 1024UL * 1024UL; // use all spi flash
	cfg.phys_addr = 0;                     // start spiffs at start of spi flash
	cfg.phys_erase_block = 65536;          // according to datasheet
	cfg.log_block_size = 65536;            // let us not complicate things
	cfg.log_page_size = LOG_PAGE_SIZE;     // as we said
#endif

	cfg.hal_read_f = spiffs_drv.read;
	cfg.hal_write_f = spiffs_drv.write;
	cfg.hal_erase_f = spiffs_drv.erase;
    
	res = SPIFFS_mount(&fs,
		&cfg,
		spiffs_work_buf,
		spiffs_fds,
		sizeof(spiffs_fds),
		spiffs_cache_buf,
		sizeof(spiffs_cache_buf),
		0);
	if (res == SPIFFS_ERR_NOT_A_FS)
	{
		res = SPIFFS_format(&fs);
		res = SPIFFS_mount(&fs,
			&cfg,
			spiffs_work_buf,
			spiffs_fds,
			sizeof(spiffs_fds),
			spiffs_cache_buf,
			sizeof(spiffs_cache_buf),
			0);
	}
	printf("mount res: %i\n", res);
}

//--------------------------------------------
void spiffs_test()
{
	char buf[12];
	spiffs_file fd;

	if ((fd = SPIFFS_open(&fs, "my_file", SPIFFS_RDWR, 0)) == SPIFFS_ERR_NOT_FOUND)
	{
		fd = SPIFFS_open(&fs, "my_file", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);
		if (SPIFFS_write(&fs, fd, (u8_t *)"Hello world", 12) < 0)
		{
			printf("errno %li\n", SPIFFS_errno(&fs));
		}
		SPIFFS_close(&fs, fd);
		fd = SPIFFS_open(&fs, "my_file", SPIFFS_RDWR, 0);
	}
	if (SPIFFS_read(&fs, fd, (u8_t *)buf, 12) < 0)
	{
		printf("errno %li\n", SPIFFS_errno(&fs));
	}
	SPIFFS_close(&fs, fd);
	printf("--> %s <--\n", buf);
}

//--------------------------------------------
int main(void)
{
	platform_init();
	spiffs_drv.init();

	spiffs_mount();
	spiffs_test();

	while (1);
	return 0;
}
