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

/*
* Some of this code has been taken from the steel_ne article:
* http://we.easyelectronics.ru/electro-and-pc/stm32-uip-enc28j60.html
*/

#include "platform.h"
#include "hal-enc28j60.h"
#include "enc28j60-regs.h"
#include "eth-uip-drv.h"

#define ENC28J60_RXSIZE             0x1A00
#define ENC28J60_MAXFRAME           1500
#define ENC28J60_RXSTART            0x0000
#define ENC28J60_RXEND             (ENC28J60_RXSIZE - 1)
#define ENC28J60_TXSTART            ENC28J60_RXSIZE

//--------------------------------------------
static void enc28j60_write_op(uint8_t op, uint8_t addr, uint8_t data)
{
	hal_enc28j60_select();
	hal_enc28j60_tx(op | (addr & ENC28J60_ADDR_MASK));
	hal_enc28j60_tx(data);
	hal_enc28j60_release();
}

//--------------------------------------------
static uint8_t enc28j60_read_op(uint8_t op, uint8_t addr)
{
	uint8_t data;

	hal_enc28j60_select();
	hal_enc28j60_tx(op | (addr & ENC28J60_ADDR_MASK));
	if (addr & ENC28J60_SPRD_MASK)
	{
		// 4.2.1 READ CONTROL REGISTER COMMAND
		// If the address specifies one of the MAC or MII registers,
		// a dummy byte will first be shifted out on the SO pin.
		hal_enc28j60_rx();
	}
	data = hal_enc28j60_rx();
	hal_enc28j60_release();
	return data;
}

//--------------------------------------------
static void enc28j60_set_bank(uint8_t addr)
{
	uint8_t bank;
	static uint8_t enc28j60_bank = 0;

	if ((addr & ENC28J60_ADDR_MASK) < ENC28J60_COMMON_CR)
	{
		bank = (addr & ENC28J60_BANK_MASK) >> ENC28J60_BANK_MASK_SHIFT;
		if (bank != enc28j60_bank)
		{
			enc28j60_write_op(ENC28J60_SPI_BFC, ECON1, ECON1_BSEL1 | ECON1_BSEL0);
			enc28j60_write_op(ENC28J60_SPI_BFS, ECON1, bank);
			enc28j60_bank = bank;
		}
	}
}

//--------------------------------------------
// 4.2.1 READ CONTROL REGISTER COMMAND
static uint8_t enc28j60_rcr(uint8_t addr)
{
	enc28j60_set_bank(addr);
	return enc28j60_read_op(ENC28J60_SPI_RCR, addr);
}
#if 0
static uint16_t enc28j60_rcr16(uint8_t addr)
{
	enc28j60_set_bank(addr);
	return enc28j60_read_op((uint16_t)ENC28J60_SPI_RCR, addr) | ((uint16_t)enc28j60_read_op(ENC28J60_SPI_RCR, addr + 1) << 8);
}
#endif

//--------------------------------------------
// 4.2.2 READ BUFFER MEMORY COMMAND
static void enc28j60_rbm(uint8_t *buf, uint16_t buflen)
{
	hal_enc28j60_select();
	hal_enc28j60_tx(ENC28J60_SPI_RBM);
	while (buflen--)
	{
		*(buf++) = hal_enc28j60_rx();
	}
	hal_enc28j60_release();
}

//--------------------------------------------
// 4.2.3 WRITE CONTROL REGISTER COMMAND
static void enc28j60_wcr(uint8_t addr, uint8_t data)
{
	enc28j60_set_bank(addr);
	enc28j60_write_op(ENC28J60_SPI_WCR, addr, data);
}
static void enc28j60_wcr16(uint8_t addr, uint16_t data)
{
	enc28j60_set_bank(addr);
	enc28j60_write_op(ENC28J60_SPI_WCR, addr, (uint8_t)(data & 0xFF));
	enc28j60_write_op(ENC28J60_SPI_WCR, addr + 1, (uint8_t)(data >> 8));
}

//--------------------------------------------
// 4.2.4 WRITE BUFFER MEMORY COMMAND
static void enc28j60_wbm(const uint8_t *buf, uint16_t buflen)
{
	hal_enc28j60_select();
	hal_enc28j60_txrx(ENC28J60_SPI_WBM);
	while (buflen--)
	{
		hal_enc28j60_tx(*(buf++));
	}
	hal_enc28j60_release();
}

//--------------------------------------------
// 4.2.5 BIT FIELD SET COMMAND
static void enc28j60_bfs(uint8_t addr, uint8_t mask)
{
	enc28j60_set_bank(addr);
	enc28j60_write_op(ENC28J60_SPI_BFS, addr, mask);
}

//--------------------------------------------
// 4.2.6 BIT FIELD CLEAR COMMAND
static void enc28j60_bfc(uint8_t addr, uint8_t mask)
{
	enc28j60_set_bank(addr);
	enc28j60_write_op(ENC28J60_SPI_BFC, addr, mask);
}

//--------------------------------------------
// 4.2.7 SYSTEM RESET COMMAND
static void enc28j60_src(void)
{
	hal_enc28j60_select();
	hal_enc28j60_txrx(ENC28J60_SPI_SRC);
	hal_enc28j60_release();
}

#if 0
//--------------------------------------------
// 3.3.1 READING PHY REGISTERS
static uint16_t enc28j60_read_phy(uint8_t addr)
{
	// Write the address of the PHY register to read
	// from into the MIREGADR register.
	enc28j60_wcr(MIREGADR, addr);
	// Set the MICMD.MIIRD bit. The read operation
	// begins and the MISTAT.BUSY bit is set.	
	enc28j60_bfs(MICMD, MICMD_MIIRD);
	// Wait 10.24 us. Poll the MISTAT.BUSY bit to be
	// certain that the operation is complete. While
	// busy, the host controller should not start any
	// MIISCAN operations or write to the MIWRH register.
	// When the MAC has obtained the register
	// contents, the BUSY bit will clear itself.
	while (enc28j60_rcr(MISTAT) & MISTAT_BUSY);
	// Clear the MICMD.MIIRD bit.
	enc28j60_bfc(MICMD, MICMD_MIIRD);
	// Read the desired data from the MIRDL and
	// MIRDH registers. The order that these bytes are
	// accessed is unimportant.
	return enc28j60_rcr16(MIRDL);
}
#endif

//--------------------------------------------
// 3.3.2 WRITING PHY REGISTERS
static void enc28j60_write_phy(uint8_t addr, uint16_t data)
{
	// Write the address of the PHY register to write to into the MIREGADR register.
	enc28j60_wcr(MIREGADR, addr);
	// Write the lower 8 bits of data to write into the MIWRL register.
	// Write the upper 8 bits of data to write into the MIWRH register.
	// Writing to this register automatically begins the MIIM transaction,
	// so it must be written to after MIWRL. The MISTAT.BUSY bit becomes set.
	enc28j60_wcr16(MIWRL, data);
	// The PHY register will be written after the MIIM operation
	// completes, which takes 10.24 ?s. When the write
	// operation has completed, the BUSY bit will clear itself.
	while (enc28j60_rcr(MISTAT) & MISTAT_BUSY);
}


//--------------------------------------------
// 6.0 INITIALIZATION
void enc28j60_init(const uint8_t *mac)
{
	hal_enc28j60_init();
	enc28j60_src();
	// Errata: 2. Module: Reset
	// After issuing the SPI Reset command, wait at least
	// 1 ms in firmware for the device to be ready.
	delay_ms(2);

	// 6.1 Receive Buffer
	// Before receiving any packets, the receive buffer must
	// be initialized by programming the ERXST and ERXND
	// Pointers.
	enc28j60_wcr16(ERXSTL, ENC28J60_RXSTART);
#if 0
	enc28j60_rcr16(ERXSTL);
#endif
	enc28j60_wcr16(ERXNDL, ENC28J60_RXEND);
	// For tracking purposes, the ERXRDPT registers should
	// additionally be programmed with the same value. To
	// program ERXRDPT, the host controller must write to
	// ERXRDPTL first, followed by ERXRDPTH.
	enc28j60_wcr16(ERXRDPTL, ENC28J60_RXSTART);


#if 0
	// 6.3 Receive Filters
	// For broadcast packets we allow only ARP packtets
	// All other packets should be unicast only for our mac (MAADR)
	// The pattern to match on is therefore
	// Type     ETH.DST
	// ARP      BROADCAST
	// 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
	// in binary these poitions are:11 0000 0011 1111
	// This is hex 303F->EPMM0=0x3f,EPMM1=0x30
	enc28j60_wcr(ERXFCON, ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_PMEN);
	enc28j60_wcr(EPMM0, 0x3F);
	enc28j60_wcr(EPMM1, 0x30);
	enc28j60_wcr(EPMCSL, 0xF9);
	enc28j60_wcr(EPMCSH, 0xF7);
	// Errata: 18. Module: Receive Filter
	// If using the Pattern Match receive filter, some
	// packets may be accepted that should be rejected.
	// Work around: When using the pattern match receive filter, 
	// discard any unwanted packets in software.
#endif

	// 6.5 MAC Initialization Settings
	// 1. Set the MARXEN bit in MACON1 to enable the
	// MAC to receive frames. If using full duplex, most
	// applications should also set TXPAUS and
	// RXPAUS to allow IEEE defined flow control to function.
	enc28j60_wcr(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS);

	// bring MAC out of reset
	enc28j60_wcr(MACON2, 0x00);

	// 2. Configure the PADCFG, TXCRCEN and FULDPX bits of MACON3.
	// enable automatic padding to 60bytes and CRC operations
	// Most applications should enable automatic padding to at least
	// 60 bytes and always append a valid CRC (MACON3_PADCFG0)
	// For convenience, many applications may wish to set
	// the FRMLNEN bit as well to enable frame length
	// status reporting. The FULDPX bit should be set
	// if the application will be connected to a
	// full-duplex configured remote node; otherwise, it should be left clear.
	enc28j60_wcr(MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN | MACON3_FULDPX);

	// 3. Configure the bits in MACON4. For conformance
	// to the IEEE 802.3 standard, set the DEFER bit (applies to half duplex only).
#if 0
	enc28j60_wcr(MACON4, MACON4_DEFER);
#endif

	// 4. Program the MAMXFL registers with the maximum
	// frame length to be permitted to be received
	// or transmitted. Normal network nodes are
	// designed to handle packets that are 1518 bytes or less.
	enc28j60_wcr16(MAMXFLL, ENC28J60_MAXFRAME);

	// 5. Configure the Back-to-Back Inter-Packet Gap
	// register, MABBIPG. Most applications will program
	// this register with 15h when Full-Duplex
	// mode is used and 12h when Half-Duplex mode is used.
	enc28j60_wcr(MABBIPG, 0x15);

	// 6. Configure the Non-Back-to-Back Inter-Packet
	// Gap register low byte, MAIPGL. Most applications
	// will program this register with 12h.
	enc28j60_wcr(MAIPGL, 0x12);

	// 7. If half duplex is used, the Non-Back-to-Back
	// Inter-Packet Gap register high byte, MAIPGH,
	// should be programmed. Most applications will
	// program this register to 0Ch.
	enc28j60_wcr(MAIPGH, 0x0C);

	// 8. If Half-Duplex mode is used, program the
	// Retransmission and Collision Window registers,
	// MACLCON1 and MACLCON2. Most applications
	// will not need to change the default Reset values.
	// If the network is spread over exceptionally long
	// cables, the default value of MACLCON2 may need to be increased.

	// 9. Program the local MAC address into the
	// MAADR1:MAADR6 registers.
	enc28j60_wcr(MAADR5, mac[0]);
	enc28j60_wcr(MAADR4, mac[1]);
	enc28j60_wcr(MAADR3, mac[2]);
	enc28j60_wcr(MAADR2, mac[3]);
	enc28j60_wcr(MAADR1, mac[4]);
	enc28j60_wcr(MAADR0, mac[5]);

	// 6.6 PHY Initialization Settings
	// For proper duplex operation, the PHCON1.PDPXMD
	// bit must also match the value of the MACON3.FULDPX bit.
	enc28j60_write_phy(PHCON1, PHCON1_PDPXMD);
	// If using half duplex, the host controller may wish to set
	// the PHCON2.HDLDIS bit to prevent automatic
	// loopback of the data which is transmitted.
	enc28j60_write_phy(PHCON2, PHCON2_HDLDIS);

	// enable packet reception
	enc28j60_bfs(ECON1, ECON1_RXEN);
}

//--------------------------------------------
// 7.1 Transmitting Packets
void enc28j60_send_packet(const uint8_t *buf, uint16_t buflen)
{
	// Check no transmit in progress
	while (enc28j60_rcr(ECON1) & ECON1_TXRTS)
	{
		// Errata: 12. Module: Transmit Logic
		// In Half-Duplex mode, a hardware transmission
		// abort caused by excessive collisions, a late collision
		// or excessive deferrals, may stall the internal
		// transmit logic. The next packet transmit initiated by
		// the host controller may never succeed
		// (ECON1.TXRTS will remain set indefinitely).
		if (enc28j60_rcr(EIR) & EIR_TXERIF)
		{
			enc28j60_bfs(ECON1, ECON1_TXRST);
			enc28j60_bfc(ECON1, ECON1_TXRST);
		}
	}

	// 1. Appropriately program the ETXST Pointer to
	// point to an unused location in memory. It will
	// point to the per packet control byte. It is
	// recommended that an even address be used for ETXST.
	enc28j60_wcr16(ETXSTL, ENC28J60_TXSTART);

	// 2. Use the WBM SPI command to write the per
	// packet control byte, the destination address, the
	// source MAC address, the type/length and the data payload.
	enc28j60_wcr16(EWRPTL, ENC28J60_TXSTART);
	// per packet control byte (0x00 means use macon3 settings)
	enc28j60_wbm((uint8_t *)("0x00"), 1);
	// packet data
	enc28j60_wbm(buf, buflen);

	// 3. Appropriately program the ETXND Pointer. It
	// should point to the last byte in the data payload.
	enc28j60_wcr16(ETXNDL, ENC28J60_TXSTART + buflen);

	// 5. Start the transmission process by setting ECON1.TXRTS.
	enc28j60_bfs(ECON1, ECON1_TXRTS);
}

//--------------------------------------------
// 7.2 Receiving Packets
uint16_t enc28j60_recv_packet(uint8_t *buf, uint16_t buflen)
{
	uint8_t buf2byte[2];
	uint16_t rxstat;
	uint16_t len;
	static uint16_t enc28j60_rxptr = ENC28J60_RXSTART;

	if (!enc28j60_rcr(EPKTCNT))
	{
		return 0;
	}

	// set the read pointer to the start of the received packet
	enc28j60_wcr16(ERDPTL, enc28j60_rxptr);

	// 7.2.2 RECEIVE PACKET LAYOUT
	// read the next packet pointer
	enc28j60_rbm(buf2byte, sizeof(buf2byte));
	enc28j60_rxptr = (uint16_t)buf2byte[0] | ((uint16_t)buf2byte[1] << 8);
	// read the packet length
	enc28j60_rbm(buf2byte, sizeof(buf2byte));
	len = (uint16_t)buf2byte[0] | ((uint16_t)buf2byte[1] << 8);
	// read the receive status
	enc28j60_rbm(buf2byte, sizeof(buf2byte));
	rxstat = (uint16_t)buf2byte[0] | ((uint16_t)buf2byte[1] << 8);

	if (rxstat & 0x80)
	{
		// TABLE 7-3: RECEIVE STATUS VECTORS
		// Received Ok
		// Indicates that at the packet had a valid CRC and no symbol errors.

		// crc (4 bytes) is not needed
		len -= 4;
		if (len > buflen)
		{
			len = buflen;
		}
		enc28j60_rbm(buf, len);    
	}
	else
	{
		len = 0;
	}

	// Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	// Errata: 14. Module: Memory (Ethernet Buffer)
	// Ensure that only odd addresses are written to the
	// ERXRDPT registers. Assuming that ERXND contains
	// an odd value, many applications can derive a
	// suitable value to write to ERXRDPT by subtracting
	// one from the Next Packet Pointer (a value always
	// ensured to be even because of hardware padding)
	// and then compensating for a potential ERXST to ERXND wrap-around.
	if (enc28j60_rxptr == ENC28J60_RXSTART)
	{
		enc28j60_wcr16(ERXRDPTL, ENC28J60_RXEND);
	}
	else
	{
		enc28j60_wcr16(ERXRDPTL, (enc28j60_rxptr - 1));
	}

	// decrement the packet counter indicate we are done with this packet
	enc28j60_bfs(ECON2, ECON2_PKTDEC);

	return len;
}
