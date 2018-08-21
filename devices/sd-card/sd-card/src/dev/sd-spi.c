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
#include "hal-sd-spi.h"
#include "sd-defs.h"
#include "sd.h"

#define INIT_PROCESS_COUNT        1000      // Number of attempts to get a ready state
#define DRESP_ATTEMPTS_COUNT      100000    // Number of attempts to get a data response
#define READY_ATTEMPTS_COUNT      100000    // Number of attempts to get a ready state after programming
#define RESP_ATTEMPTS_COUNT       1000      // Number of attempts to get a command response

static sd_info_t sdinfo;

//--------------------------------------------
// The CRC7 is a 7-bit value with polynomial:
// G(x) = x^7 + x^3 + 1
static uint8_t crc7_one(uint8_t crc, uint8_t data)
{
	uint8_t cnt;
	const uint8_t g = 0x89;

	crc ^= data;
	for (cnt = 0; cnt < 8; cnt++)
	{
		if (crc & 0x80)
		{
			crc ^= g;
		}
		crc <<= 1;
	}
	return crc;
}
static uint8_t crc7(const uint8_t *buf, uint8_t len)
{
	uint8_t cnt;
	uint8_t crc = 0;

	for (cnt = 0; cnt < len; cnt++)
	{
		crc = crc7_one(crc, buf[cnt]);
	}
	return crc >> 1;
}

//--------------------------------------------
// The CRC16 is a 16-bit value with polynomial:
// G(x) = x^16 + x^12 + x^5 + 1
static uint16_t crc16_one(uint16_t crc, uint8_t data)
{
	crc = (uint8_t)(crc >> 8) | (crc << 8);
	crc ^= data;
	crc ^= (uint8_t)(crc & 0xFF) >> 4;
	crc ^= (crc << 8) << 4;
	crc ^= ((crc & 0xFF) << 4) << 1;
	return crc;
}
static uint16_t crc16(const uint8_t *buf, uint16_t len)
{
	uint16_t cnt;
	uint16_t crc = 0;

	for (cnt = 0; cnt < len; cnt++)
	{
		crc = crc16_one(crc, buf[cnt]);
	}
	return crc;
}

//--------------------------------------------
static sd_error send_command(uint32_t cmd, uint32_t arg, sd_resp resp, uint8_t *buf)
{
	uint8_t frame[6];
	uint32_t cnt;
	uint16_t resp_len;
	uint8_t resp_r1;

	// it's needed for old SD
	hal_sd_spi_txrx(0xFF);
	hal_sd_spi_txrx(0xFF);

	switch(resp)
	{
	case RESP_R1:
	case RESP_R1b:
		resp_len = 1;
		break;
	case RESP_R2:
		resp_len = 2;
		break;
	case RESP_R3:
	case RESP_R7:
		resp_len = 5;
		break;
	default:
		return sd_err_not_supported;
	}

	frame[0] = (cmd | 0x40);
	frame[1] = (uint8_t)(arg >> 24);
	frame[2] = (uint8_t)(arg >> 16);
	frame[3] = (uint8_t)(arg >> 8);
	frame[4] = (uint8_t)(arg);
	frame[5] = (crc7(&frame[0], 5) << 1) | 0x01;

	for (cnt = 0; cnt < 6; cnt++)
	{
		hal_sd_spi_txrx(frame[cnt]);
	}

	if (cmd == CMD12)
	{
		// skip the garbage byte
		resp_r1 = hal_sd_spi_txrx(0xFF);
	}

	for (cnt = 0, resp_r1 = 0xFF; (cnt < RESP_ATTEMPTS_COUNT) && (resp_r1 & SPI_RESP_R1_ATTR); cnt++)
	{
		resp_r1 = hal_sd_spi_txrx(0xFF);
	}

	if (cnt == RESP_ATTEMPTS_COUNT)
	{
		return sd_err_ctimeout;
	}
	if (resp_r1 & SPI_RESP_R1_ILLG_CMD)
	{
		return sd_err_unexpected_command;
	}
	if (resp_r1 & SPI_RESP_R1_CRC_ERR)
	{
		return sd_err_ccrcfail;
	}

	buf[0] = resp_r1;
	for (cnt = 1; cnt < resp_len; cnt++)
	{
		buf[cnt] = hal_sd_spi_txrx(0xFF);
	}

	return sd_err_ok;
}

//--------------------------------------------
static sd_error read_data(uint8_t *buf, uint16_t len)
{
	uint32_t cnt;
	uint16_t crc;
	uint16_t calc_crc;
	uint8_t resp;

	for (cnt = 0, resp = 0xFF; (cnt < DRESP_ATTEMPTS_COUNT) && (resp != SPI_START_BLOCK_READ); cnt++)
	{
		resp = hal_sd_spi_txrx(0xFF);
	}
	if (cnt == DRESP_ATTEMPTS_COUNT)
	{
		// Timeout or Data Error Token
		return sd_err_dtimeout;
	}

	for (cnt = 0; cnt < len; cnt++)
	{
		buf[cnt] = hal_sd_spi_txrx(0xFF);
	}

	crc = hal_sd_spi_txrx(0xFF) << 8;
	crc |= hal_sd_spi_txrx(0xFF);
	calc_crc = crc16(buf, len);
	if (crc != calc_crc)
	{
		return sd_err_dcrcfail;
	}

	return sd_err_ok;
}

//--------------------------------------------
static sd_error write_data_token(uint8_t token)
{
	uint32_t cnt;
	uint8_t resp;

	hal_sd_spi_txrx(token);

	if (token == SPI_STOP_MULTI_BLOCK_WRITE)
	{
		for (cnt = 0, resp = 0x00; (cnt < READY_ATTEMPTS_COUNT) && (resp != 0xFF); cnt++)
		{
			resp = hal_sd_spi_txrx(0xFF);
		}
		if (cnt == READY_ATTEMPTS_COUNT)
		{
			// Timeout
			return sd_err_dtimeout;
		}
	}

	return sd_err_ok;
}

//--------------------------------------------
static sd_error write_data(const uint8_t *buf, uint16_t len)
{
	uint32_t cnt;
	uint16_t calc_crc;
	uint8_t resp;

	calc_crc = crc16(buf, len);

	for (cnt = 0; cnt < len; cnt++)
	{
		hal_sd_spi_txrx(buf[cnt]);
	}
	hal_sd_spi_txrx(calc_crc >> 8);
	hal_sd_spi_txrx(calc_crc);

	for (cnt = 0, resp = 0xFF; (cnt < DRESP_ATTEMPTS_COUNT) && (resp == 0xFF); cnt++)
	{
		resp = hal_sd_spi_txrx(0xFF);
	}
	if (cnt == DRESP_ATTEMPTS_COUNT)
	{
		// Timeout
		return sd_err_dtimeout;
	}
	resp &= SPI_WRITE_STATUS_MASK;
	if (resp == SPI_WRITE_STATUS_CRC_ERR)
	{
		return sd_err_dcrcfail;
	}
	if (resp == SPI_WRITE_STATUS_WRITE_ERR)
	{
		return sd_err_writefail;
	}

	for (cnt = 0, resp = 0x00; (cnt < READY_ATTEMPTS_COUNT) && (resp != 0xFF); cnt++)
	{
		resp = hal_sd_spi_txrx(0xFF);
	}
	if (cnt == READY_ATTEMPTS_COUNT)
	{
		// Timeout
		return sd_err_dtimeout;
	}

	return sd_err_ok;
}

//--------------------------------------------
// Reads and parses the Operation Conditions Register (OCR)
static sd_error read_ocr(void)
{
	sd_error err;
	uint8_t buf[6];

	// CMD58: Reads the OCR register of a card
	if ((err = send_command(CMD58, 0, RESP_R3, &buf[0])) != sd_err_ok)
	{
		return err;
	}

	// Card power up status bit (busy)
	if (buf[1] & SPI_RESP_R3_BUSY)
	{
		// Card Capacity Status (CCS)
		if (buf[1] & SPI_RESP_R3_CCS)
		{
			// SDHC or SDXC, it will be known later
			sdinfo.type = CARD_SDHC;
		}
		else
		{
			sdinfo.type = CARD_SDSC;
		}
	}

	return sd_err_ok;
}

//--------------------------------------------
// Reads and parses the SD Configuration Register (SCR)
static sd_error read_scr(void)
{
	sd_error err;
	uint8_t buf[8];
	uint8_t sd_sec;

	// CMD55: Next command is an application specific
	if ((err = send_command(CMD55, 0, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	// ACMD51: Reads the SD Configuration Register (SCR)
	if ((err = send_command(ACMD51, 0, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	if ((err = read_data(&buf[0], 8)) != sd_err_ok)
	{
		return err;
	}

	sd_sec = buf[1] & SPI_SCR_SD_SECURITY_MASK;
	if (sd_sec == SPI_SCR_SD_SECURITY_SDSC)
	{
		sdinfo.type = CARD_SDSC;
	}
	if (sd_sec == SPI_SCR_SD_SECURITY_SDHC)
	{
		sdinfo.type = CARD_SDHC;
	}
	if (sd_sec == SPI_SCR_SD_SECURITY_SDXC)
	{
		sdinfo.type = CARD_SDXC;
	}

	return sd_err_ok;
}

//--------------------------------------------
// Reads and parses the Card-Specific Data register (CSD)
static sd_error read_csd(void)
{
	sd_error err;
	uint8_t buf[16];
	uint32_t c_size;
	uint8_t c_size_mult;
	uint8_t read_bl_len;

	// CMD9: Asks the selected card to send its cardspecific data (CSD)
	if ((err = send_command(CMD9, 0, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	if ((err = read_data(&buf[0], 16)) != sd_err_ok)
	{
		return err;
	}

	if (sdinfo.type == CARD_SDSC)
	{
		// CSD Version 1.0

		read_bl_len = buf[5] & 0x0F;

		c_size = ((uint32_t)buf[6] & 0x03) << 10;
		c_size |= (uint32_t)buf[7] << 2;
		c_size |= ((uint32_t)buf[8] & 0xD0) >> 6;

		c_size_mult = (buf[9] & 0x03) << 1;
		c_size_mult |= (buf[10] & 0x80) >> 7;

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

		c_size = ((uint32_t)buf[7] & 0x3F) << 16;
		c_size |= (uint32_t)buf[8] << 8;
		c_size |= (uint32_t)buf[9];

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
static void reset_prepare(void)
{
	uint8_t cnt;

	// A device shall be ready to accept the first command within 1ms from detecting VDD min.
	// Device may use up to 74 clocks for preparation before receiving the first command.
	hal_sd_spi_release();
	for (cnt = 0; cnt < 10; cnt++)
	{
		hal_sd_spi_txrx(0xFF);
	}
}

//--------------------------------------------
static sd_error reset_idle(void)
{
	sd_error err;
	uint8_t buf[16];
	uint32_t cnt;
	uint32_t resp_r7;

	// In case of SD host, CMD0 is not necessary.
	// In case of SPI host, CMD0 shall be the first
	// command to send the card to SPI mode.
	if ((err = send_command(CMD0, 0, RESP_R1, &buf[0])) != sd_err_ok)
	{
		return err;
	}

	// PLSS version 2.00 or later host shall issue
	// CMD8 and verify voltage before card initialization.
	err = send_command(CMD8, CMD8_VHS1 | CMD8_CHECK, RESP_R7, &buf[0]);
	if ((err != sd_err_unexpected_command) && (err != sd_err_ok))
	{
		return err;
	}
	if (err == sd_err_unexpected_command)
	{
		// PLSS version 1.xx
		for (cnt = 0; (cnt < INIT_PROCESS_COUNT) && (buf[0] & SPI_RESP_R1_IDLE_STATE); cnt++)
		{
			// CMD55: Next command is an application specific
			if ((err = send_command(CMD55, 0, RESP_R1, &buf[0])) != sd_err_ok)
			{
				return err;
			}
			// ACMD41 activates the card's initialization process.
			if ((err = send_command(ACMD41, 0, RESP_R1, &buf[0])) != sd_err_ok)
			{
				return err;
			}
			delay_us(100);
		}
		if (cnt == INIT_PROCESS_COUNT)
		{
			return sd_err_not_supported;
		}

		sdinfo.type = CARD_SDSC;
	}
	else
	{
		// PLSS version 2.00 or later
		resp_r7 = ((uint32_t)buf[1] << 24) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 8) | (uint32_t)buf[4];
		if (resp_r7 != (uint32_t)(CMD8_VHS1 | CMD8_CHECK))
		{
			// voltage not supported
			return sd_err_not_supported;
		}

		// 2.7-3.6V supported
		for (cnt = 0; (cnt < INIT_PROCESS_COUNT) && (buf[0] & SPI_RESP_R1_IDLE_STATE); cnt++)
		{
			// CMD55: Next command is an application specific
			if ((err = send_command(CMD55, 0, RESP_R1, &buf[0])) != sd_err_ok)
			{
				return err;
			}
			// ACMD41 sends host capacity support information (HCS)
			// and activates the card's initialization process.
			if ((err = send_command(ACMD41, ACMD41_HCS, RESP_R1, &buf[0])) != sd_err_ok)
			{
				return err;
			}
			delay_us(10);
		}
		if (cnt == INIT_PROCESS_COUNT)
		{
			return sd_err_not_supported;
		}

		// Reads and parses the Operation Conditions Register (OCR)
		if ((err = read_ocr()) != sd_err_ok)
		{
			return err;
		}
	}

	if (sdinfo.type == CARD_SDSC)
	{
		// CMD16: In case of SDSC Card, block length is set by this command to 512 bytes
		// In case of SDHC and SDXC Cards, block length of the memory access commands are fixed to 512 bytes
		if ((err = send_command(CMD16, 512, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}

	// Reads and parses the SD Configuration Register (SCR)
	if ((err = read_scr()) != sd_err_ok)
	{
		return err;
	}

	// Reads and parses the Card-Specific Data register (CSD)
	if ((err = read_csd()) != sd_err_ok)
	{
		return err;
	}

	return sd_err_ok;
}

//--------------------------------------------
static sd_error read(uint8_t *rxbuf, uint32_t sector, uint32_t count)
{
	sd_error err;
	uint8_t buf[2];
	uint32_t cnt;

	if ((err = send_command(CMD13, 0, RESP_R2, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	if (buf[0] != 0 || buf[1] != 0)
	{
		return sd_err_wrong_status;
	}

	if (sdinfo.type == CARD_SDSC)
	{
		// SDSC uses the 32-bit argument of memory access commands as byte address format
		// Block length is determined by CMD16
		sector *= 512;
	}

	if (count == 1)
	{
		// CMD17: Reads a block of the size selected by the SET_BLOCKLEN command (SDSC)
		// or exactly 512 byte (SDHC and SDXC)
		if ((err = send_command(CMD17, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}
	else
	{
		// CMD18: Continuously transfers data blocks from card to host 
		// until interrupted by a STOP_TRANSMISSION command
		if ((err = send_command(CMD18, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}

	for (cnt = 0; cnt < count; rxbuf += 512, cnt++)
	{
		if ((err = read_data(rxbuf, 512)) != sd_err_ok)
		{
			return err;
		}
	}

	if (count > 1)
	{
		// CMD12: Forces the card to stop transmission
		// if count == 1, "operation complete" is automatic
		if ((err = send_command(CMD12, 0, RESP_R1b, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}

	return sd_err_ok;
}

//--------------------------------------------
static sd_error write(const uint8_t *txbuf, uint32_t sector, uint32_t count)
{
	sd_error err;
	uint8_t buf[4];
	uint32_t cnt;

	if ((err = send_command(CMD13, 0, RESP_R2, &buf[0])) != sd_err_ok)
	{
		return err;
	}
	if (buf[0] != 0 || buf[1] != 0)
	{
		return sd_err_wrong_status;
	}

	if (sdinfo.type == CARD_SDSC)
	{
		// SDSC uses the 32-bit argument of memory access commands as byte address format
		// Block length is determined by CMD16
		sector *= 512;
	}

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
		if ((err = send_command(CMD55, 0, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
		// ACMD23: Set the number of write blocks to be pre-erased before writing 
		// (to be used for faster Multiple Block WR command)
		if ((err = send_command(ACMD23, count, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
		// CMD25: Continuously writes blocks of data until a Stop Multiple Block Write token follows.
		if ((err = send_command(CMD25, sector, RESP_R1, &buf[0])) != sd_err_ok)
		{
			return err;
		}
	}

	for (cnt = 0; cnt < count; txbuf += 512, cnt++)
	{
		if (count == 1)
		{
			write_data_token(SPI_START_SINGLE_BLOCK_WRITE);
		}
		else
		{
			write_data_token(SPI_START_MULTI_BLOCK_WRITE);
		}
		if ((err = write_data(txbuf, 512)) != sd_err_ok)
		{
			return err;
		}
	}

	if (count > 1)
	{
		// Forces the card to stop receiving
		// if count == 1, "transfer end" is automatic
		write_data_token(SPI_STOP_MULTI_BLOCK_WRITE);
	}

	return sd_err_ok;
}

//--------------------------------------------
sd_error sd_spi_reset(void)
{
	sd_error err;

	hal_sd_spi_slow();
	reset_prepare();
	hal_sd_spi_select();
	err = reset_idle();
	hal_sd_spi_release();
	if (err == sd_err_ok)
	{
		hal_sd_spi_fast();
	}
	return err;
}

//--------------------------------------------
sd_error sd_spi_read(uint8_t *rxbuf, uint32_t sector, uint32_t count)
{
	sd_error err;

	hal_sd_spi_select();
	err = read(rxbuf, sector, count);
	hal_sd_spi_release();
	return err;
}

//--------------------------------------------
sd_error sd_spi_write(const uint8_t *txbuf, uint32_t sector, uint32_t count)
{
	sd_error err;

	hal_sd_spi_select();
	err = write(txbuf, sector, count);
	hal_sd_spi_release();
	return err;
}

//--------------------------------------------
sd_info_t* sd_spi_getcardinfo(void)
{
	return &sdinfo;
}
