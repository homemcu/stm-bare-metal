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
#include "hal-pcd8544.h"
#include "pcd8544-cmds.h"
#include "mono-scr-drv.h"

static uint8_t buf[(PCD8544_SCR_W * PCD8544_SCR_H) / 8];

//--------------------------------------------
static void pcd8544_command(uint8_t cmd)
{
	hal_pcd8544_command();
	hal_pcd8544_tx(cmd);
}

//--------------------------------------------
void pcd8544_init(void)
{
	hal_pcd8544_init();
	hal_pcd8544_select();

	// Function set: extended instruction set control
	pcd8544_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	// set Bias System
	pcd8544_command(PCD8544_SETBIAS | 0x04);
	// write VOP to register
	pcd8544_command(PCD8544_SETVOP | 0x48);
	// Function set: normal instruction set control
	pcd8544_command(PCD8544_FUNCTIONSET);
	// sets display configuration: normal mode
	pcd8544_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

	// waiting for the end of the transfer
	delay_us(3);
	hal_pcd8544_release();
}

//--------------------------------------------
void pcd8544_flush(void)
{
	uint8_t col;
	uint8_t bank;

	hal_pcd8544_select();

	for (bank = 0; bank < 6; bank++)
	{
		// Set Y address of RAM (0 <= Y <= 5)
		pcd8544_command(PCD8544_SETYADDR | bank);
		// Set X address of RAM (0 <= X <= 83)
		pcd8544_command(PCD8544_SETXADDR | 0x00);

		// waiting for the end of the transfer
		delay_us(3);
		hal_pcd8544_data();

		for (col = 0; col < PCD8544_SCR_W; col++)
		{
			hal_pcd8544_tx(buf[(PCD8544_SCR_W * bank) + col]);
		}
	}
	pcd8544_command(PCD8544_SETYADDR);

	// waiting for the end of the transfer
	delay_us(3);
	hal_pcd8544_release();
}

//--------------------------------------------
uint8_t* pcd8544_getbufaddr(void)
{
	return buf;
}
