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
#include "stm32f7xx-hw.h"
#include "sd-defs.h"
#include "sd.h"

//--------------------------------------------
// SDMMC(PLL48CLK, SYSCLK)
// GPIO_AF12_SDMMC1
// D0:   PC8
// D1:   PC9
// D2:   PC10
// D3:   PC11
// D4:   PB8
// D5:   PB9
// D6:   PC6
// D7:   PC7
// CK:   PC12
// CMD:  PD2
//
// The first clock source in the brackets
// is selected as SDMMC clock by default,
// see RCC->DCKCFGR2 register

//--------------------------------------------
#define PORT_SDMMC_D0          GPIO_C   // PC8  <-> DO0
#define PIN_SDMMC_D0           8
#define PORT_SDMMC_D1          GPIO_C   // PC9  <-> DO1
#define PIN_SDMMC_D1           9
#define PORT_SDMMC_D2          GPIO_C   // PC10 <-> DO2
#define PIN_SDMMC_D2           10
#define PORT_SDMMC_D3          GPIO_C   // PC11 <-> DO3
#define PIN_SDMMC_D3           11
#define PORT_SDMMC_CK          GPIO_C   // PC12 --> CKL
#define PIN_SDMMC_CK           12
#define PORT_SDMMC_CMD         GPIO_D   // PD2  --> CMD
#define PIN_SDMMC_CMD          2

//--------------------------------------------
// SDMMC Initialization Frequency (400kHz max)
#define SDMMC_INIT_CLK_DIV         118   // SDMMC_CK = PLL48CLK(48MHz) / (118 + 2) = 400kHz
// SDMMC Data Transfer Frequency (25MHz max)
#define SDMMC_TRANSFER_CLK_DIV     0     // SDMMC_CK = PLL48CLK(48MHz) / (0 + 2) = 24MHz
//#define SDMMC_TRANSFER_CLK_DIV     118     // SDMMC_CK = PLL48CLK(48MHz) / (118 + 2) = 400kHz

#if 0
#define SDMMC_IRQ_PREEMPT_PRIORITY 0
#endif

#if 0
#define DATA_BUS_WIDTH_1
#endif

//--------------------------------------------
#define INIT_PROCESS_COUNT        1000      // Number of attempts to get a ready state
#define DATATIMEOUT               0xFFFFFF  // A method for computing a realistic values from CSD is described in the specs

#define SDMMC_ICR_FLAGS (SDMMC_ICR_CCRCFAILC | SDMMC_ICR_DCRCFAILC | \
                         SDMMC_ICR_CTIMEOUTC | SDMMC_ICR_DTIMEOUTC | \
                         SDMMC_ICR_TXUNDERRC | SDMMC_ICR_RXOVERRC  | \
                         SDMMC_ICR_CMDRENDC  | SDMMC_ICR_CMDSENTC  | \
                         SDMMC_ICR_DATAENDC  | SDMMC_ICR_DBCKENDC)

static sd_info_t sdinfo;

//--------------------------------------------
void hal_sd_sdmmc_init(void)
{
	// IO port C clock enable
	// IO port D clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
	// SDMMC1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SDMMC1EN;

	// configure operation mode of GPIO pins
	hw_cfg_pin(GPIOx(PORT_SDMMC_D0),   PIN_SDMMC_D0,   GPIOCFG_MODE_ALT | GPIO_AF12_SDMMC1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_SDMMC_D1),   PIN_SDMMC_D1,   GPIOCFG_MODE_ALT | GPIO_AF12_SDMMC1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_SDMMC_D2),   PIN_SDMMC_D2,   GPIOCFG_MODE_ALT | GPIO_AF12_SDMMC1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_SDMMC_D3),   PIN_SDMMC_D3,   GPIOCFG_MODE_ALT | GPIO_AF12_SDMMC1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_SDMMC_CK),   PIN_SDMMC_CK,   GPIOCFG_MODE_ALT | GPIO_AF12_SDMMC1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_SDMMC_CMD),  PIN_SDMMC_CMD,  GPIOCFG_MODE_ALT | GPIO_AF12_SDMMC1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

#if 0
	// Enable all interrupts
	SDMMC1->MASK = SDMMC_MASK_RXOVERRIE | SDMMC_MASK_TXUNDERRIE | SDMMC_MASK_DCRCFAILIE | SDMMC_MASK_DTIMEOUTIE | SDMMC_MASK_DATAENDIE;
	// set sdmmc1 global interrupt priority
	NVIC_SetPriority(SDMMC1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), SDMMC_IRQ_PREEMPT_PRIORITY, 0));
	// enable sdmmc1 global interrupt
	NVIC_EnableIRQ(SDMMC1_IRQn);
#endif
}

//--------------------------------------------
void hal_sd_sdmmc_init_dma(void)
{
	// DMA2 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// DMA stream disabled
	DMA2_Stream3->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream3->CR & DMA_SxCR_EN);

	DMA2_Stream3->CR =  DMA_SxCR_CHSEL_2  | // Channel selection: (100) channel 4
	                    DMA_SxCR_MBURST_0 | // Memory burst transfer configuration: (01) INCR4
	                    DMA_SxCR_PBURST_0 | // Peripheral burst transfer configuration: (01) INCR4
	                                        // Current target: (0) ignored
	                                        // Double buffer mode: (0) No buffer switching at the end of transfer
	                                        // Priority level: (00) Low
	                                        // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_1  | // Memory data size: (10) 32-bit
	                    DMA_SxCR_PSIZE_1  | // Peripheral data size: (10) 32-bit
	                    DMA_SxCR_MINC     | // Memory increment mode: (1) incremented after each data transfer
	                                        // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                                        // Circular mode: (0) disabled
	                                        // Data transfer direction: (00) Peripheral-to-memory
	                    DMA_SxCR_PFCTRL;    // Peripheral flow controller: (1) The peripheral is the flow controller

	DMA2_Stream3->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO


	// DMA stream disabled
	DMA2_Stream6->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream6->CR & DMA_SxCR_EN);

	DMA2_Stream6->CR =  DMA_SxCR_CHSEL_2  | // Channel selection: (100) channel 4
	                    DMA_SxCR_MBURST_0 | // Memory burst transfer configuration: (01) INCR4
	                    DMA_SxCR_PBURST_0 | // Peripheral burst transfer configuration: (01) INCR4
	                                        // Current target: (0) ignored
	                                        // Double buffer mode: (0) No buffer switching at the end of transfer
	                                        // Priority level: (00) Low
	                                        // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_1  | // Memory data size: (10) 32-bit
	                    DMA_SxCR_PSIZE_1  | // Peripheral data size: (10) 32-bit
	                    DMA_SxCR_MINC     | // Memory increment mode: (1) incremented after each data transfer
	                                        // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                                        // Circular mode: (0) disabled
	                    DMA_SxCR_DIR_0    | // Data transfer direction: (01) Memory-to-peripheral
	                    DMA_SxCR_PFCTRL;    // Peripheral flow controller: (1) The peripheral is the flow controller

	DMA2_Stream6->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 0
	// set sdmmc1 dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), SDMMC_IRQ_PREEMPT_PRIORITY, 0));
	// enable sdmmc1 dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream3_IRQn);
	// set sdmmc1 dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), SDMMC_IRQ_PREEMPT_PRIORITY, 0));
	// enable sdmmc1 dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream6_IRQn);
#endif
}

//--------------------------------------------
static void sdmmc_slow(void)
{
	// SDIO_CK clock enable
	SDMMC1->CLKCR = SDMMC_INIT_CLK_DIV | SDMMC_CLKCR_CLKEN | SDMMC_CLKCR_NEGEDGE;
	// Power-on: the card is clocked
	SDMMC1->POWER = SDMMC_POWER_PWRCTRL_0 | SDMMC_POWER_PWRCTRL_1;
#if 0
	delay_us(1);
#endif
}

//--------------------------------------------
static void sdmmc_fast(void)
{
#ifdef DATA_BUS_WIDTH_1
	// SDIO_CK clock and bus width change
	SDMMC1->CLKCR = SDMMC_TRANSFER_CLK_DIV | SDMMC_CLKCR_CLKEN | SDMMC_CLKCR_NEGEDGE;
#else
	// SDIO_CK clock and bus width change
	SDMMC1->CLKCR = SDMMC_TRANSFER_CLK_DIV | SDMMC_CLKCR_WIDBUS_0 | SDMMC_CLKCR_CLKEN | SDMMC_CLKCR_NEGEDGE;
#endif
#if 0
	// Power-on: the card is clocked
	SDMMC1->POWER = SDMMC_POWER_PWRCTRL_0 | SDMMC_POWER_PWRCTRL_1;
#endif
#if 0
	delay_us(1);
#endif
}

//--------------------------------------------
static sd_error send_command(uint32_t cmd, uint32_t arg, sd_resp resp, uint32_t *buf)
{
	uint32_t cmd_waitresp;

	switch(resp)
	{
	case RESP_NONE:
		// No response, expect CMDSENT flag
		cmd_waitresp = 0;
		break;
	case RESP_R1:
	case RESP_R1b:
	case RESP_R3:
	case RESP_R6:
	case RESP_R7:
		// Short response, expect CMDREND or CCRCFAIL flag
		cmd_waitresp = SDMMC_CMD_WAITRESP_0;
		break;
	case RESP_R2:
		// Long response, expect CMDREND or CCRCFAIL flag
		cmd_waitresp = SDMMC_CMD_WAITRESP_0 | SDMMC_CMD_WAITRESP_1;
		break;
	}

	SDMMC1->ARG = arg;
	SDMMC1->CMD = (uint32_t)(cmd & SDMMC_CMD_CMDINDEX) | (cmd_waitresp & SDMMC_CMD_WAITRESP) | SDMMC_CMD_CPSMEN;

	if (resp == RESP_NONE)
	{
		while (!(SDMMC1->STA & SDMMC_STA_CMDSENT));
		SDMMC1->ICR = SDMMC_ICR_CMDSENTC;
		return sd_err_ok;
	}

	while (!(SDMMC1->STA & (SDMMC_STA_CMDREND | SDMMC_STA_CCRCFAIL | SDMMC_STA_CTIMEOUT)));
	if (SDMMC1->STA & SDMMC_STA_CTIMEOUT)
	{
		SDMMC1->ICR = SDMMC_STA_CTIMEOUT;
		return sd_err_ctimeout;
	}

	// STM32F74xxx STM32F75xxx Errata sheet:
	// 2.12.1 Wrong CCRCFAIL status after a response without CRC is received (RESP_R3)
	if ((resp != RESP_R3) && (SDMMC1->STA & SDMMC_STA_CCRCFAIL))
	{
		SDMMC1->ICR = SDMMC_STA_CCRCFAIL;
		return sd_err_ccrcfail;
	}
	SDMMC1->ICR = SDMMC_STA_CMDREND | SDMMC_STA_CCRCFAIL;

	switch(resp)
	{
	case RESP_R1:
	case RESP_R1b:
	case RESP_R3:
	case RESP_R6:
	case RESP_R7:
		buf[0] = SDMMC1->RESP1;
		break;
	case RESP_R2:
		buf[0] = SDMMC1->RESP1;
		buf[1] = SDMMC1->RESP2;
		buf[2] = SDMMC1->RESP3;
		buf[3] = SDMMC1->RESP4;
		break;
	default:
		break;
	}

	switch(resp)
	{
	case RESP_R1:
	case RESP_R1b:
	case RESP_R6:
	case RESP_R7:
		if (SDMMC1->RESPCMD != cmd)
		{
			return sd_err_unexpected_command;
		}
		break;
	default:
		break;
	}

	return sd_err_ok;
}

//--------------------------------------------
// Reads and parses the SD Configuration Register (SCR),
// calls only in Transfer State (tran)
static sd_error read_scr(void)
{
	sd_error err;
	uint32_t cnt;
	uint32_t buf[2];
	uint32_t sd_sec;

	err = sd_err_ok;
	cnt = 0;

	// CMD16: Block length to 8 byte
	if ((err = send_command(CMD16, 8, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}

	// CMD55: Next command is an application specific
	if ((err = send_command(CMD55, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}

	// Receiving 8 data bytes
	SDMMC1->DTIMER = DATATIMEOUT;
	SDMMC1->DLEN = 8;
	SDMMC1->DCTRL = (SDMMC_DCTRL_DBLOCKSIZE_0 | SDMMC_DCTRL_DBLOCKSIZE_1) | \
	                 SDMMC_DCTRL_DTDIR | \
	                 SDMMC_DCTRL_DTEN;

	// ACMD51: Reads the SD Configuration Register (SCR)
	if ((err = send_command(ACMD51, 0, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}

	while (!(SDMMC1->STA & (SDMMC_STA_DBCKEND | SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT)))
	{
		if (SDMMC1->STA & SDMMC_STA_RXDAVL)
		{
			buf[cnt++] = SDMMC1->FIFO;
		}
	}

	if (SDMMC1->STA & SDMMC_STA_DTIMEOUT)
	{
		err = sd_err_dtimeout;
	}
	if (SDMMC1->STA & SDMMC_STA_DCRCFAIL)
	{
		err = sd_err_dcrcfail;
	}
	if (SDMMC1->STA & SDMMC_STA_RXOVERR)
	{
		err = sd_err_rxoverr;
	}

	SDMMC1->ICR = SDMMC_ICR_FLAGS;
	SDMMC1->DCTRL = 0;

	// swap bytes
	buf[0] = (buf[0] & 0x0000FFFF) << 16 | (buf[0] & 0xFFFF0000) >> 16;
	buf[0] = (buf[0] & 0x00FF00FF) << 8  | (buf[0] & 0xFF00FF00) >> 8;
	buf[1] = (buf[1] & 0x0000FFFF) << 16 | (buf[1] & 0xFFFF0000) >> 16;
	buf[1] = (buf[1] & 0x00FF00FF) << 8  | (buf[1] & 0xFF00FF00) >> 8;

	sd_sec = buf[0] & SCR_SD_SECURITY_MASK;
	if (sd_sec == SCR_SD_SECURITY_SDSC)
	{
		sdinfo.type = CARD_SDSC;
	}
	if (sd_sec == SCR_SD_SECURITY_SDHC)
	{
		sdinfo.type = CARD_SDHC;
	}
	if (sd_sec == SCR_SD_SECURITY_SDXC)
	{
		sdinfo.type = CARD_SDXC;
	}

	return err;
}

//--------------------------------------------
// Reads and parses the Card-Specific Data register (CSD)
static sd_error read_csd(void)
{
	sd_error err;
	uint32_t buf[4];
	uint32_t c_size;
	uint8_t c_size_mult;
	uint8_t read_bl_len;

	if ((err = send_command(CMD9, sdinfo.rca << 16, RESP_R2, &buf[0])) != sd_err_ok)
	{
		return err;
	}

	if (sdinfo.type == CARD_SDSC)
	{
		// CSD Version 1.0

		read_bl_len = (uint8_t)(buf[1] & 0x000F0000) >> 16;

		c_size  = (buf[1] & 0x00000300) << 2;
		c_size |= (buf[1] & 0x000000FF) << 2;
		c_size |= (buf[2] & 0xD0000000) >> 30;

		c_size_mult  = (buf[2] & 0x00030000) >> 15;
		c_size_mult |= (buf[2] & 0x00008000) >> 15;

		// The memory capacity of the card is computed from the entries
		// C_SIZE, C_SIZE_MULT and READ_BL_LEN as follows:
		// memory capacity = BLOCKNR * BLOCK_LEN (in bytes)
		// memory capacity = BLOCKNR * BLOCK_LEN / 512 (in 512 byte sectors)
		// Where
		// BLOCKNR = (C_SIZE+1) * MULT
		// MULT = 2^(C_SIZE_MULT+2)
		// BLOCK_LEN = 2^READ_BL_LEN

		sdinfo.card_size = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << read_bl_len) / 512;
		sdinfo.sect_size = 512;
		// The erase operation can erase either one or multiple sectors
		sdinfo.erase_size = 1;
	}
	else
	{
		// CSD Version 2.0

		c_size  = (buf[1] & 0x0000003F) << 16;
		c_size |= (buf[2] & 0xFF000000) >> 16;
		c_size |= (buf[2] & 0x00FF0000) >> 16;

		// The user data area capacity is calculated from C_SIZE as follows:
		// memory capacity = (C_SIZE+1) * 512 (in kilobytes)
		// memory capacity = (C_SIZE+1) * 512 * 1024 (in bytes)
		// memory capacity = (C_SIZE+1) * 512 * 1024 / 512 = (C_SIZE+1) * 1024 (in 512 byte sectors)

		sdinfo.card_size = (c_size + 1) * 1024;
		sdinfo.sect_size = 512;
		// The erase operation can erase either one or multiple sectors
		sdinfo.erase_size = 1;
	}

	return sd_err_ok;
}

//--------------------------------------------
sd_info_t* hal_sd_sdmmc_getcardinfo(void)
{
	return &sdinfo;
}

//--------------------------------------------
sd_error hal_sd_sdmmc_reset(void)
{
	sd_error err;
	uint32_t cnt;
	uint32_t buf[4];

	sdmmc_slow();

#if 1
	// In case of SD host, CMD0 is not necessary.
	// In case of SPI host, CMD0 shall be the first
	// command to send the card to SPI mode.
	send_command(CMD0, 0, RESP_NONE, &buf[0]);
#endif
	// The version 2.00 or later host shall issue
	// CMD8 and verify voltage before card initialization.
	err = send_command(CMD8, CMD8_VHS1 | CMD8_CHECK, RESP_R7, &buf[0]);
	if ((err != sd_err_unexpected_command) && (err != sd_err_ok))
	{
		return err;
	}
	if (err == sd_err_unexpected_command)
	{
		// PLSS version 1.xx
		for (cnt = 0; cnt < INIT_PROCESS_COUNT; cnt++)
		{
			// CMD55: Next command is an application specific
			if ((err = send_command(CMD55, 0, RESP_R1, &buf[0])) != sd_err_ok)
			{
				return err;
			}
			// ACMD41 is a synchronization command used to negotiate the operation voltage range
			// and to poll the cards until they are out of their power-up sequence.
			if ((err = send_command(ACMD41, ACMD41_32_33V, RESP_R3, &buf[0])) != sd_err_ok)
			{
				return err;
			}
			if (buf[0] & RESP_R3_BUSY)
			{
				sdinfo.type = CARD_SDSC;
				break;
			}
		}
		if (cnt == INIT_PROCESS_COUNT)
		{
			return sd_err_not_supported;
		}
	}
	else
	{
		// PLSS version 2.00 or later
		if (buf[0] != (uint32_t)(CMD8_VHS1 | CMD8_CHECK))
		{
			// voltage not supported
			return sd_err_not_supported;
		}

		// 2.7-3.6V supported
		for (cnt = 0; cnt < INIT_PROCESS_COUNT; cnt++)
		{
			// CMD55: Next command is an application specific
			if ((err = send_command(CMD55, 0, RESP_R1, &buf[0])) != sd_err_ok)
			{
				return err;
			}
			// ACMD41 is a synchronization command used to negotiate the operation voltage range
			// and to poll the cards until they are out of their power-up sequence.
			// ACMD41: Sends host capacity support information (HCS) and asks the
			// accessed card to send its operating condition register (OCR) content in the
			// response on the CMD line.
			if ((err = send_command(ACMD41, ACMD41_HCS | ACMD41_32_33V, RESP_R3, &buf[0])) != sd_err_ok)
			{
				return err;
			}
			if (buf[0] & RESP_R3_BUSY)
			{
				if (buf[0] & RESP_R3_CCS)
				{
					// SDHC or SDXC, it will be known later
					sdinfo.type = CARD_SDHC;
				}
				else
				{
					sdinfo.type = CARD_SDSC;
				}
				break;
			}
		}
		if (cnt == INIT_PROCESS_COUNT)
		{
			return sd_err_not_supported;
		}
	}

	// CMD2: Asks any card to send the CID numbers on the CMD line
	if ((err = send_command(CMD2, 0, RESP_R2, &buf[0])) != sd_err_ok)
	{
		return err;
	}

	// CMD3: Ask the card to publish a new relative address (RCA)
	// ==> Stand-by State (stby)
	if ((err = send_command(CMD3, 0, RESP_R6, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	sdinfo.rca = buf[0] >> 16;

	// CMD9: Reads and parses the Card-Specific Data register (CSD)
	if ((err = read_csd()) != sd_err_ok)
	{
		return err;
	}

	// CMD7: Command toggles a card between the stand-by and transfer states
	// ==> Transfer State (tran)
	if ((err = send_command(CMD7, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}

	// CMD16->CMD55->ACMD51: Reads and parses the SD Configuration Register (SCR)
	if ((err = read_scr()) != sd_err_ok)
	{
		return err;
	}

#ifndef DATA_BUS_WIDTH_1
	// CMD55: Next command is an application specific
	if ((err = send_command(CMD55, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	// ACMD6: Defines the 4 bits data bus width to be used for data transfer
	if ((err = send_command(ACMD6, 0x2, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
#endif

	sdmmc_fast();

	return sd_err_ok;
}

//--------------------------------------------
sd_error hal_sd_sdmmc_read(uint8_t *rxbuf, uint32_t sector, uint32_t count)
{
	sd_error err;
	uint32_t buf[4];
	uint32_t *dbuf;
	uint32_t cnt;
	uint32_t sta;

	dbuf = (uint32_t *)&rxbuf[0];

	// CMD13: Asks the selected card to send its status register
	if ((err = send_command(CMD13, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	// not Transfer State (tran)
	if ((buf[0] & RESP_R1_CURRENT_STATE_MASK) != RESP_R1_CURRENT_STATE_TRAN)
	{
		return sd_err_wrong_status;
	}

	if (sdinfo.type == CARD_SDSC)
	{
		// SDSC uses the 32-bit argument of memory access commands as byte address format
		// Block length is determined by CMD16, by default - 512 byte
		sector *= 512;
	}

	// Receiving 512 bytes data blocks
	SDMMC1->DTIMER = DATATIMEOUT;
	SDMMC1->DLEN = count * 512;
	SDMMC1->DCTRL = (SDMMC_DCTRL_DBLOCKSIZE_0 | SDMMC_DCTRL_DBLOCKSIZE_3) | // Data block size: (1001) 512 bytes
	                                     // (0) DMA disabled
	                 SDMMC_DCTRL_DTDIR | // Data transfer direction selection: (1) From card to controller
	                 SDMMC_DCTRL_DTEN;   // (1) Data transfer enabled bit

	if (count == 1)
	{
		// CMD17: Reads a block of the size selected by the SET_BLOCKLEN command (SDSC)
		// or exactly 512 byte (SDHC and SDXC)
		// ==> Sending-data State (data)
		if ((err = send_command(CMD17, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}
	else
	{
		// CMD18: Continuously transfers data blocks from card to host 
		// until interrupted by a STOP_TRANSMISSION command
		// ==> Sending-data State (data)
		if ((err = send_command(CMD18, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}

	while (!(SDMMC1->STA & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DATAEND)))
	{
		while (SDMMC1->STA & SDMMC_STA_RXFIFOHF)
		{
			for (cnt = 0; cnt < 8; cnt++)
			{
				*(dbuf + cnt) = SDMMC1->FIFO;
			}
			dbuf += 8;
		}
	}

	SDMMC1->DCTRL = 0;

	if (count > 1)
	{
		// CMD12: Forces the card to stop transmission
		// if count == 1, "operation complete" is automatic
		// ==> Transfer State (tran)
		if ((err = send_command(CMD12, 0, RESP_R1b, &buf[0])) != sd_err_ok)
		{
			SDMMC1->ICR = SDMMC_ICR_FLAGS;
			return err;
		}
	}

	sta = SDMMC1->STA;
	SDMMC1->ICR = SDMMC_ICR_FLAGS;

	if (sta & SDMMC_STA_DTIMEOUT)
	{
		return sd_err_dtimeout;
	}
	if (sta & SDMMC_STA_DCRCFAIL)
	{
		return sd_err_dcrcfail;
	}
	if (sta & SDMMC_STA_RXOVERR)
	{
		return sd_err_rxoverr;
	}

	return sd_err_ok;
}

//--------------------------------------------
sd_error hal_sd_sdmmc_write(const uint8_t *txbuf, uint32_t sector, uint32_t count)
{
	sd_error err;
	uint32_t buf[4];
	uint32_t *dbuf;
	uint32_t cnt;
	uint32_t sta;

	dbuf = (uint32_t *)&txbuf[0];

	// CMD13: Asks the selected card to send its status register
	if ((err = send_command(CMD13, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	// not Transfer State (tran)
	if ((buf[0] & RESP_R1_CURRENT_STATE_MASK) != RESP_R1_CURRENT_STATE_TRAN)
	{
		return sd_err_wrong_status;
	}

	if (sdinfo.type == CARD_SDSC)
	{
		// SDSC uses the 32-bit argument of memory access commands as byte address format
		// Block length is determined by CMD16, by default - 512 byte
		sector *= 512;
	}

	// Sending 512 bytes data blocks
	SDMMC1->DTIMER = DATATIMEOUT;
	SDMMC1->DLEN = count * 512;
	SDMMC1->DCTRL = (SDMMC_DCTRL_DBLOCKSIZE_0 | SDMMC_DCTRL_DBLOCKSIZE_3) | // Data block size: (1001) 512 bytes
	                                     // (0) DMA disabled
	                                     // Data transfer direction selection: (0) From controller to card
	                 SDMMC_DCTRL_DTEN;   // (1) Data transfer enabled bit

	if (count == 1)
	{
		// CMD24: Write a block of the size selected by the SET_BLOCKLEN command (SDSC)
		// or exactly 512 byte (SDHC and SDXC)
		// ==> Receive-data State (rcv)
		if ((err = send_command(CMD24, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}
	else
	{
		// CMD55: Next command is an application specific
		if ((err = send_command(CMD55, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
		// ACMD23: Set the number of write blocks to be pre-erased before writing 
		// (to be used for faster Multiple Block WR command)
		if ((err = send_command(ACMD23, count, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
		// CMD25: Continuously writes blocks of data until a STOP_TRANSMISSION follows.
		// ==> Receive-data State (rcv)
		if ((err = send_command(CMD25, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}

	while (!(SDMMC1->STA & (SDMMC_STA_TXUNDERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DATAEND)))
	{
		while (SDMMC1->STA & SDMMC_STA_TXFIFOHE)
		{
			for (cnt = 0; cnt < 8; cnt++)
			{
				SDMMC1->FIFO = *(dbuf + cnt);
			}
			dbuf += 8;
		}
	}

	SDMMC1->DCTRL = 0;

	if (count > 1)
	{
		// CMD12: Forces the card to stop receiving
		// if count == 1, "transfer end" is automatic
		// ==> Programming State (prg) ==> Transfer State (tran)
		if ((err = send_command(CMD12, 0, RESP_R1b, &buf[0])) != sd_err_ok)
		{
			SDMMC1->ICR = SDMMC_ICR_FLAGS;
			return err;
		}
	}

	sta = SDMMC1->STA;
	SDMMC1->ICR = SDMMC_ICR_FLAGS;

	if (sta & SDMMC_STA_DTIMEOUT)
	{
		return sd_err_dtimeout;
	}
	if (sta & SDMMC_STA_DCRCFAIL)
	{
		return sd_err_dcrcfail;
	}
	if (sta & SDMMC_STA_TXUNDERR)
	{
		return sd_err_txunderr;
	}

	return sd_err_ok;
}

//--------------------------------------------
// rxbuf address MUST be aligned to uint32_t width
sd_error hal_sd_sdmmc_read_dma(uint8_t *rxbuf, uint32_t sector, uint32_t count)
{
	sd_error err;
	uint32_t buf[4];
	uint32_t sta;

	// CMD13: Asks the selected card to send its status register
	if ((err = send_command(CMD13, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	// not Transfer State (tran)
	if ((buf[0] & RESP_R1_CURRENT_STATE_MASK) != RESP_R1_CURRENT_STATE_TRAN)
	{
		return sd_err_wrong_status;
	}

	// DMA stream disabled
	DMA2_Stream3->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream3->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA2->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3 | DMA_LIFCR_CHTIF3;

	// Set the DMA addresses
	DMA2_Stream3->PAR = (uint32_t)&(SDMMC1->FIFO);
	DMA2_Stream3->M0AR = (uint32_t)rxbuf;

	// Set the number of 32-bit words to transfer
	DMA2_Stream3->NDTR = 512 * count / sizeof(uint32_t);

	// DMA stream enabled
	DMA2_Stream3->CR |= DMA_SxCR_EN;
	while (!(DMA2_Stream3->CR & DMA_SxCR_EN));

	if (sdinfo.type == CARD_SDSC)
	{
		// SDSC uses the 32-bit argument of memory access commands as byte address format
		// Block length is determined by CMD16, by default - 512 byte
		sector *= 512;
	}

	// Receiving 512 bytes data blocks
	SDMMC1->DTIMER = DATATIMEOUT;
	SDMMC1->DLEN = count * 512;
	SDMMC1->DCTRL = (SDMMC_DCTRL_DBLOCKSIZE_0 | SDMMC_DCTRL_DBLOCKSIZE_3) | // Data block size: (1001) 512 bytes
	                 SDMMC_DCTRL_DMAEN | // (1) DMA enabled
	                 SDMMC_DCTRL_DTDIR;  // Data transfer direction selection: (1) From card to controller
	                                     // (0) Data transfer enabled bit

	if (count == 1)
	{
		// CMD17: Reads a block of the size selected by the SET_BLOCKLEN command (SDSC)
		// or exactly 512 byte (SDHC and SDXC)
		// ==> Sending-data State (data)
		if ((err = send_command(CMD17, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}
	else
	{
		// CMD18: Continuously transfers data blocks from card to host 
		// until interrupted by a STOP_TRANSMISSION command
		// ==> Sending-data State (data)
		if ((err = send_command(CMD18, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}

	// SDIO data transfer enabled
	SDMMC1->DCTRL |= SDMMC_DCTRL_DTEN;

	while (!(SDMMC1->STA & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DATAEND)));

	SDMMC1->DCTRL = 0;

	if (count > 1)
	{
		// CMD12: Forces the card to stop transmission
		// if count == 1, "operation complete" is automatic
		// ==> Transfer State (tran)
		if ((err = send_command(CMD12, 0, RESP_R1b, &buf[0])) != sd_err_ok)
		{
			SDMMC1->ICR = SDMMC_ICR_FLAGS;
			return err;
		}
	}

	sta = SDMMC1->STA;
	SDMMC1->ICR = SDMMC_ICR_FLAGS;

	if (sta & SDMMC_STA_DTIMEOUT)
	{
		return sd_err_dtimeout;
	}
	if (sta & SDMMC_STA_DCRCFAIL)
	{
		return sd_err_dcrcfail;
	}
	if (sta & SDMMC_STA_RXOVERR)
	{
		return sd_err_rxoverr;
	}

	return sd_err_ok;
}

//--------------------------------------------
// txbuf address MUST be aligned to uint32_t width
sd_error hal_sd_sdmmc_write_dma(const uint8_t *txbuf, uint32_t sector, uint32_t count)
{
	sd_error err;
	uint32_t buf[4];
	uint32_t sta;

	// CMD13: Asks the selected card to send its status register
	if ((err = send_command(CMD13, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	// not Transfer State (tran)
	if ((buf[0] & RESP_R1_CURRENT_STATE_MASK) != RESP_R1_CURRENT_STATE_TRAN)
	{
		return sd_err_wrong_status;
	}

	// DMA stream disabled
	DMA2_Stream6->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream6->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA2->HIFCR = DMA_HIFCR_CTCIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CFEIF6 | DMA_HIFCR_CHTIF6;

	// Set the DMA addresses
	DMA2_Stream6->PAR = (uint32_t)&(SDMMC1->FIFO);
	DMA2_Stream6->M0AR = (uint32_t)txbuf;

	// Set the number of 32-bit words to transfer
	DMA2_Stream6->NDTR = 512 * count / sizeof(uint32_t);

	// DMA stream enabled
	DMA2_Stream6->CR |= DMA_SxCR_EN;
	while (!(DMA2_Stream6->CR & DMA_SxCR_EN));

	if (sdinfo.type == CARD_SDSC)
	{
		// SDSC uses the 32-bit argument of memory access commands as byte address format
		// Block length is determined by CMD16, by default - 512 byte
		sector *= 512;
	}

	// Sending 512 bytes data blocks
	SDMMC1->DTIMER = DATATIMEOUT;
	SDMMC1->DLEN = count * 512;
	SDMMC1->DCTRL = (SDMMC_DCTRL_DBLOCKSIZE_0 | SDMMC_DCTRL_DBLOCKSIZE_3) | // Data block size: (1001) 512 bytes
	                 SDMMC_DCTRL_DMAEN;  // (1) DMA enabled
	                                     // Data transfer direction selection: (0) From controller to card
	                                     // (0) Data transfer enabled bit

	if (count == 1)
	{
		// CMD24: Write a block of the size selected by the SET_BLOCKLEN command (SDSC)
		// or exactly 512 byte (SDHC and SDXC)
		// ==> Receive-data State (rcv)
		if ((err = send_command(CMD24, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}
	else
	{
		// CMD55: Next command is an application specific
		if ((err = send_command(CMD55, sdinfo.rca << 16, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
		// ACMD23: Set the number of write blocks to be pre-erased before writing 
		// (to be used for faster Multiple Block WR command)
		if ((err = send_command(ACMD23, count, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
		// CMD25: Continuously writes blocks of data until a STOP_TRANSMISSION follows.
		// ==> Receive-data State (rcv)
		if ((err = send_command(CMD25, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}

	// SDIO data transfer enabled
	SDMMC1->DCTRL |= SDMMC_DCTRL_DTEN;

	while (!(SDMMC1->STA & (SDMMC_STA_TXUNDERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DATAEND)));

	SDMMC1->DCTRL = 0;

	if (count > 1)
	{
		// CMD12: Forces the card to stop receiving
		// if count == 1, "transfer end" is automatic
		// ==> Programming State (prg) ==> Transfer State (tran)
		if ((err = send_command(CMD12, 0, RESP_R1b, &buf[0])) != sd_err_ok)
		{
			SDMMC1->ICR = SDMMC_ICR_FLAGS;
			return err;
		}
	}

	sta = SDMMC1->STA;
	SDMMC1->ICR = SDMMC_ICR_FLAGS;

	if (sta & SDMMC_STA_DTIMEOUT)
	{
		return sd_err_dtimeout;
	}
	if (sta & SDMMC_STA_DCRCFAIL)
	{
		return sd_err_dcrcfail;
	}
	if (sta & SDMMC_STA_TXUNDERR)
	{
		return sd_err_txunderr;
	}

	return sd_err_ok;
}

#if 0
//--------------------------------------------
void DMA2_Stream3_IRQHandler(void)
{
	if (DMA2->LISR & DMA_LISR_TCIF3)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
	}
	if (DMA2->LISR & DMA_LISR_TEIF3)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTEIF3;
	}
	if (DMA2->LISR & DMA_LISR_FEIF3)
	{
		DMA2->LIFCR |= DMA_LIFCR_CFEIF3;
	}
}
//--------------------------------------------
void DMA2_Stream6_IRQHandler(void)
{
	if (DMA2->HISR & DMA_HISR_TCIF6)
	{
		DMA2->HIFCR |= DMA_HIFCR_CTCIF6;
	}
	if (DMA2->HISR & DMA_HISR_TEIF6)
	{
		DMA2->HIFCR |= DMA_HIFCR_CTEIF6;
	}
	if (DMA2->HISR & DMA_HISR_FEIF6)
	{
		DMA2->HIFCR |= DMA_HIFCR_CFEIF6;
	}
}
#endif
#if 0
//--------------------------------------------
void SDMMC1_IRQHandler(void)
{
	if (SDMMC1->STA & SDMMC_STA_RXOVERR)
	{
		SDMMC1->ICR = SDMMC_ICR_RXOVERRC;
	}
	if (SDMMC1->STA & SDMMC_STA_TXUNDERR)
	{
		SDMMC1->ICR = SDMMC_ICR_TXUNDERRC;
	}
	if (SDMMC1->STA & SDMMC_STA_DCRCFAIL)
	{
		SDMMC1->ICR = SDMMC_ICR_DCRCFAILC;
	}
	if (SDMMC1->STA & SDMMC_STA_DTIMEOUT)
	{
		SDMMC1->ICR = SDMMC_ICR_DTIMEOUTC;
	}
	if (SDMMC1->STA & SDMMC_STA_DATAEND)
	{
		SDMMC1->ICR = SDMMC_ICR_DATAENDC;
	}
}
#endif