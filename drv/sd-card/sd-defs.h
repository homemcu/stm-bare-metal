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

#ifndef HAL_SD_DEFS_H_
#define HAL_SD_DEFS_H_

typedef enum
{
	RESP_NONE,
	RESP_R1,
	RESP_R1b,
	RESP_R2,
	RESP_R3,
	RESP_R6,
	RESP_R7
} sd_resp;

#define CMD0                          0                 // GO_IDLE_STATE          (SDIO:RESP_NONE   SPI:RESP_R1)
#define CMD2                          2                 // ALL_SEND_CID           (SDIO:RESP_R2     SPI: - )
#define CMD3                          3                 // SEND_RELATIVE_ADDR     (SDIO:RESP_R6     SPI: - )
#define CMD7                          7                 // SELECT/DESELECT_CARD   (SDIO:RESP_R1b    SPI: - )
#define CMD8                          8                 // SEND_IF_COND           (SDIO:RESP_R7     SPI:RESP_R7)
#define CMD9                          9                 // SEND_CSD               (SDIO:RESP_R2     SPI:RESP_R1+D16)
#define CMD12                         12                // STOP_TRANSMISSION      (SDIO:RESP_R1b    SPI:RESP_R1b)
#define CMD13                         13                // SEND_STATUS            (SDIO:RESP_R1     SPI:RESP_R2)
#define CMD16                         16                // SET_BLOCKLEN           (SDIO:RESP_R1     SPI:RESP_R1)
#define CMD17                         17                // READ_SINGLE_BLOCK      (SDIO:RESP_R1     SPI:RESP_R1)
#define CMD18                         18                // READ_MULTIPLE_BLOCK    (SDIO:RESP_R1     SPI:RESP_R1)
#define CMD24                         24                // WRITE_BLOCK            (SDIO:RESP_R1     SPI:RESP_R1)
#define CMD25                         25                // WRITE_MULTIPLE_BLOCK   (SDIO:RESP_R1     SPI:RESP_R1)
#define CMD55                         55                // APP_CMD                (SDIO:RESP_R1     SPI:RESP_R1)
#define CMD58                         58                // READ_OCR               (SDIO: -          SPI:RESP_R3)
#define	ACMD6                         6                 // SET_BUS_WIDTH          (SDIO:RESP_R1     SPI: - )
#define	ACMD23                        23                // SET_WR_BLK_ERASE_COUNT (SDIO:RESP_R1     SPI:RESP_R1)
#define	ACMD41                        41                // SD_SEND_OP_COND        (SDIO:RESP_R3     SPI:RESP_R1)
#define	ACMD51                        51                // SEND_SCR               (SDIO:RESP_R1+D8  SPI:RESP_R1+D8)

#define CMD8_VHS1                     0x100             // 2.7-3.6V
#define CMD8_CHECK                    0xAA              // check pattern

#define	ACMD41_HCS                    0x40000000        // Host Capacity Support (0b: SDSC Only Host, 1b: SDHC or SDXC Supported)
#define	ACMD41_33_34V                 0x00100000        // 3.3-3.4V
#define	ACMD41_32_33V                 0x00080000        // 3.2-3.3V

#define	RESP_R3_CCS                   0x40000000        // card capacity status
#define	RESP_R3_BUSY                  0x80000000        // card power up status bit
#define	SPI_RESP_R3_CCS               0x40              // card capacity status
#define	SPI_RESP_R3_BUSY              0x80              // card power up status bit

#define	SPI_WRITE_STATUS_MASK         0x1F              // Data response token: status mask
#define	SPI_WRITE_STATUS_OK           0x05              // Data response token: data accepted
#define	SPI_WRITE_STATUS_CRC_ERR      0x0B              // Data response token: crc error
#define	SPI_WRITE_STATUS_WRITE_ERR    0x0D              // Data response token: write error
#define	SPI_RESP_R1_ATTR              0x80              // This bit must be zero in r1 response
#define	SPI_RESP_R1_IDLE_STATE        0x01              // The card is in idle state and running the initializing process
#define	SPI_RESP_R1_ILLG_CMD          0x04              // illegal command
#define	SPI_RESP_R1_CRC_ERR           0x08              // com crc error
#define	SPI_RESP_R1B_BUSY             0x7F              // r1b busy

#define	RESP_R1_CURRENT_STATE_MASK    0x1E00            // The state of the card field mask
#define	RESP_R1_CURRENT_STATE_IDLE    0x0000            // idle
#define	RESP_R1_CURRENT_STATE_READY   0x0200            // ready
#define	RESP_R1_CURRENT_STATE_IDENT   0x0400            // ident
#define	RESP_R1_CURRENT_STATE_STBY    0x0600            // stby
#define	RESP_R1_CURRENT_STATE_TRAN    0x0800            // tran
#define	RESP_R1_CURRENT_STATE_DATA    0x0A00            // data
#define	RESP_R1_CURRENT_STATE_RCV     0x0C00            // rcv
#define	RESP_R1_CURRENT_STATE_PRG     0x0E00            // prg
#define	RESP_R1_CURRENT_STATE_DIS     0x1000            // dis

#define SCR_SD_BUS_WIDTHS_1           0x10000           // 1 bit (DAT0)
#define SCR_SD_BUS_WIDTHS_4           0x40000           // 4 bit (DAT0-3)
#define SCR_SD_SECURITY_MASK          0x700000          // CPRM Security Version field mask
#define SCR_SD_SECURITY_SDSC          0x200000          // SDSC Card (Security Version 1.01)
#define SCR_SD_SECURITY_SDHC          0x300000          // SDHC Card (Security Version 2.00)
#define SCR_SD_SECURITY_SDXC          0x400000          // SDXC Card (Security Version 3.xx)
#define SPI_SCR_SD_SECURITY_MASK      0x70              // CPRM Security Version field mask
#define SPI_SCR_SD_SECURITY_SDSC      0x20              // SDSC Card (Security Version 1.01)
#define SPI_SCR_SD_SECURITY_SDHC      0x30              // SDHC Card (Security Version 2.00)
#define SPI_SCR_SD_SECURITY_SDXC      0x40              // SDXC Card (Security Version 3.xx)

#define	SPI_START_BLOCK_READ          0xFE              // Start Single/Multiple Block Read
#define	SPI_START_SINGLE_BLOCK_WRITE  0xFE              // Start Single Block Write
#define	SPI_START_MULTI_BLOCK_WRITE   0xFC              // Start Multiple Block Write
#define	SPI_STOP_MULTI_BLOCK_WRITE    0xFD              // Stop Multiple Block Write

#endif // HAL_SD_DEFS_H_

