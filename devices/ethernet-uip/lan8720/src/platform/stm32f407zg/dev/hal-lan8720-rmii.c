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
#include "stm32f4xx-hw.h"
#include "lan8720-regs.h"
#include "mac.h"
#include <string.h>

//--------------------------------------------
#ifdef LWIP_FREERTOS
#include "lwipopts.h"
#include "FreeRTOSConfig.h"
#endif
//--------------------------------------------
#ifdef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
// = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY in FreeRTOSConfig.h
#define ETH_IRQ_PREEMPT_PRIORITY     configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
#else
#define ETH_IRQ_PREEMPT_PRIORITY     0
#endif
//--------------------------------------------
#ifndef CHECKSUM_BY_HARDWARE
#define CHECKSUM_BY_HARDWARE         0
#endif

//--------------------------------------------
// SMI (Station Management Interface) pins
#define PORT_ETH_MDIO         GPIO_A    // PA2  <-> ETH_MDIO
#define PIN_ETH_MDIO          2
#define PORT_ETH_MDC          GPIO_C    // PC1  --> ETH_MDC
#define PIN_ETH_MDC           1
// RMII (Reduced Media Independent Interface) pins
#define PORT_ETH_REF_CLK      GPIO_A    // PA1  <-- ETH_REF_CLK
#define PIN_ETH_REF_CLK       1
#define PORT_ETH_CRS_DV       GPIO_A    // PA7  <-- ETH_CRS_DV
#define PIN_ETH_CRS_DV        7
#define PORT_ETH_RXD0         GPIO_C    // PC4  <-- ETH_RXD0
#define PIN_ETH_RXD0          4
#define PORT_ETH_RXD1         GPIO_C    // PC5  <-- ETH_RXD1
#define PIN_ETH_RXD1          5
#define PORT_ETH_TX_EN        GPIO_B    // PB11 --> ETH_TX_EN
#define PIN_ETH_TX_EN         11
#define PORT_ETH_TXD0         GPIO_B    // PB12 --> ETH_TXD0
#define PIN_ETH_TXD0          12
#define PORT_ETH_TXD1         GPIO_B    // PB13 --> ETH_TXD1
#define PIN_ETH_TXD1          13

//--------------------------------------------
#define LAN8720_ID            0x0007C0F0
#define LAN8720A_ID           0x0007C0F1

#define BOARD_ID              LAN8720A_ID

#define MAX_PACKET_SIZE       1524
#define RX_BUF_SIZE           MAX_PACKET_SIZE
#define TX_BUF_SIZE           MAX_PACKET_SIZE
#define RX_BUFS_NUMBER        4
#define TX_BUFS_NUMBER        2

#define RDES0_OWN             0x80000000   // When set, this bit indicates that the descriptor is owned by the DMA
#define RDES0_AFM             0x40000000   // Destination address filter fail
#define RDES0_FL_MASK         0x3FFF0000   // Frame length bits mask
#define RDES0_FL_SHIFT        16           // Frame length bits shift
#define RDES0_ES              0x00008000   // Error summary
#define RDES0_FS              0x00000200   // First descriptor
#define RDES0_LS              0x00000100   // Last descriptor
#define RDES1_RCH             0x00004000   // Second address chained
#define TDES0_OWN             0x80000000   // When set, this bit indicates that the descriptor is owned by the DMA
#define TDES0_TCH             0x00100000   // Second address chained
#define TDES0_IC              0x40000000   // Interrupt on completion
#define TDES0_FS              0x10000000   // First segment
#define TDES0_LS              0x20000000   // Last segment
#define TDES0_CIC_00          0x00000000   // Checksum Insertion disabled
#define TDES0_CIC_01          0x00400000   // Only IP header checksum calculation and insertion are enabled
#define TDES0_CIC_10          0x00800000   // IP header checksum and payload checksum calculation and insertion are enabled, but pseudo-header checksum is not calculated in hardware
#define TDES0_CIC_11          0x00C00000   // IP Header checksum and payload checksum calculation and insertion are enabled, and pseudo-header checksum is calculated in hardware

//--------------------------------------------
static uint32_t phyaddr = 1 << 11;
static uint32_t clockrange;

//--------------------------------------------
typedef struct
{
	volatile uint32_t rdes0;
	uint32_t rdes1;
	uint32_t rdes2;
	uint32_t rdes3;
} norm_rx_dma_desc_t;

typedef struct
{
	volatile uint32_t rdes0;
	uint32_t rdes1;
	uint32_t rdes2;
	uint32_t rdes3;
	volatile uint32_t rdes4;
	uint32_t rdes5;
	volatile uint32_t rdes6;
	volatile uint32_t rdes7;
} enh_rx_dma_desc_t;

typedef struct
{
	volatile uint32_t tdes0;
	uint32_t tdes1;
	uint32_t tdes2;
	uint32_t tdes3;
} norm_tx_dma_desc_t;

typedef struct
{
	volatile uint32_t tdes0;
	uint32_t tdes1;
	uint32_t tdes2;
	uint32_t tdes3;
	uint32_t tdes4;
	uint32_t tdes5;
	volatile uint32_t tdes6;
	volatile uint32_t tdes7;
} enh_tx_dma_desc_t;

#if CHECKSUM_BY_HARDWARE
typedef enh_rx_dma_desc_t rx_dma_desc_t;
typedef enh_tx_dma_desc_t tx_dma_desc_t;
#else
typedef norm_rx_dma_desc_t rx_dma_desc_t;
typedef norm_tx_dma_desc_t tx_dma_desc_t;
#endif

static rx_dma_desc_t rx_dma_desc[RX_BUFS_NUMBER];
static tx_dma_desc_t tx_dma_desc[TX_BUFS_NUMBER];
DECLARE_ALIGNED_4(static uint8_t rx_buff[RX_BUFS_NUMBER][RX_BUF_SIZE]);
DECLARE_ALIGNED_4(static uint8_t tx_buff[TX_BUFS_NUMBER][TX_BUF_SIZE]);

static rx_dma_desc_t *curr_rx_dma_desc;
static tx_dma_desc_t *curr_tx_dma_desc;
static uint32_t tx_size;
static uint32_t rx_size;
static uint32_t rx_offset;

//--------------------------------------------
static void smi_clock_selection(void)
{
	// The CR clock range selection determines the HCLK frequency
	// and is used to decide the frequency of the MDC clock:
	if (SystemCoreClock >= 150000000)
	{
		clockrange = ETH_MACMIIAR_CR_Div102;
	}
	else if (SystemCoreClock >= 100000000)
	{
		clockrange = ETH_MACMIIAR_CR_Div62;
	}
	else if (SystemCoreClock >= 60000000)
	{
		clockrange = ETH_MACMIIAR_CR_Div42;
	}
	else if (SystemCoreClock >= 35000000)
	{
		clockrange = ETH_MACMIIAR_CR_Div26;
	}
	else
	{
		clockrange = ETH_MACMIIAR_CR_Div16;
	}
}

//--------------------------------------------
static void smi_write(uint32_t reg, uint32_t value)
{
	ETH->MACMIIDR = value;
	ETH->MACMIIAR = phyaddr | (reg << 6) | clockrange | ETH_MACMIIAR_MW | ETH_MACMIIAR_MB;
	while ((ETH->MACMIIAR & ETH_MACMIIAR_MB) != 0);
}

//--------------------------------------------
static uint32_t smi_read(uint32_t reg)
{
	ETH->MACMIIAR = phyaddr | (reg << 6) | clockrange | ETH_MACMIIAR_MB;
	while ((ETH->MACMIIAR & ETH_MACMIIAR_MB) != 0);
	return ETH->MACMIIDR;
}

//--------------------------------------------
static mac_error smi_find_phyaddr(void)
{
	uint32_t cnt;

	for (cnt = 0; cnt < 32; cnt++)
	{
		phyaddr = cnt << 11;
		if ( (smi_read(LAN8720_PHYI1) == (BOARD_ID >> 16)) &&
			((smi_read(LAN8720_PHYI2) & 0xFFF0) == (BOARD_ID & 0xFFF0)))
		{
			return mac_err_ok;
		}
	}
	return mac_err_phyaddr_not_found;
}


//--------------------------------------------
mac_error hal_lan8720_init(const uint8_t *mac)
{
	uint32_t cnt;
	uint32_t reg;
	mac_error error;

	// Ethernet MAC clock enable
	// Ethernet Transmission clock enable
	// Ethernet Reception clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACEN | RCC_AHB1ENR_ETHMACTXEN | RCC_AHB1ENR_ETHMACRXEN;
	// IO port A clock enable
	// IO port B clock enable
	// IO port C clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

	// configure operation mode of GPIO pins
	hw_cfg_pin(GPIOx(PORT_ETH_MDIO),    PIN_ETH_MDIO,    GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_ETH_MDC),     PIN_ETH_MDC,     GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_ETH_REF_CLK), PIN_ETH_REF_CLK, GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_ETH_CRS_DV),  PIN_ETH_CRS_DV,  GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_ETH_RXD0),    PIN_ETH_RXD0,    GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_ETH_RXD1),    PIN_ETH_RXD1,    GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_ETH_TX_EN),   PIN_ETH_TX_EN,   GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_ETH_TXD0),    PIN_ETH_TXD0,    GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_ETH_TXD1),    PIN_ETH_TXD1,    GPIOCFG_MODE_ALT | GPIO_AF11_ETH | GPIOCFG_OSPEED_VHIGH | GPIOCFG_PUPD_NONE);

	// set ethernet global interrupt priority
	NVIC_SetPriority(ETH_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), ETH_IRQ_PREEMPT_PRIORITY, 0));
	// enable ethernet global interrupt
	NVIC_EnableIRQ(ETH_IRQn);

	// system configuration controller clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// set RMII interface
	// this sets bit ETH_DMABMR_SR also, like ETH->DMABMR |= ETH_DMABMR_SR below
	SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;
#if 0
	// this is not needed
	ETH->DMABMR |= ETH_DMABMR_SR;
#endif
	while (ETH->DMABMR & ETH_DMABMR_SR);

	// MAC reset (It must be here)
	RCC->AHB1RSTR |= RCC_AHB1RSTR_ETHMACRST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_ETHMACRST;

	// set smi frequency
	smi_clock_selection();
 
	// find phy address
	if ((error = smi_find_phyaddr()) != mac_err_ok)
	{
		return error;
	}

	// soft reset
	smi_write(LAN8720_BCR, LAN8720_BCR_SR);
	while (smi_read(LAN8720_BCR) & LAN8720_BCR_SR);

	// restart auto-negotiate
	smi_write(LAN8720_BCR, smi_read(LAN8720_BCR) | LAN8720_BCR_RAN);
	// wait for auto-negotiate complete
	while (!(smi_read(LAN8720_BSR) & LAN8720_BSR_ANC));
	// wait for linked status
	while (!(smi_read(LAN8720_BSR) & LAN8720_BSR_LS));

	// MAC configuration
	ETH->MACFFR = 0;
	ETH->MACFCR = 0;
	ETH->MACVLANTR = 0;
	// set the speed and duplex mode
	reg = smi_read(LAN8720_ANLPAR);
	if (reg & (LAN8720_ANLPAR_100 | LAN8720_ANLPAR_100FULL))
		ETH->MACCR |= ETH_MACCR_FES;
	else
		ETH->MACCR &= ~ETH_MACCR_FES;
	if (reg & (LAN8720_ANLPAR_10FULL | LAN8720_ANLPAR_100FULL))
		ETH->MACCR |= ETH_MACCR_DM;
	else
		ETH->MACCR &= ~ETH_MACCR_DM;
	// set mac address
	ETH->MACA0HR = ((uint32_t)mac[5] << 8) |
				   ((uint32_t)mac[4] << 0);
	ETH->MACA0LR = ((uint32_t)mac[3] << 24) |
				   ((uint32_t)mac[2] << 16) |
				   ((uint32_t)mac[1] << 8) |
				   ((uint32_t)mac[0] << 0);


	// DMA configuration
	ETH->DMAOMR = ETH_DMAOMR_DTCEFD | ETH_DMAOMR_RSF | ETH_DMAOMR_TSF;
	ETH->DMABMR = ETH_DMABMR_AAB | ETH_DMABMR_RDP_1Beat | ETH_DMABMR_PBL_1Beat;
#if CHECKSUM_BY_HARDWARE
	// enhanced descriptor format enable
	ETH->DMABMR |= ETH_DMABMR_EDE;
#endif

	for (cnt = 0; cnt < RX_BUFS_NUMBER; cnt++)
	{
		rx_dma_desc[cnt].rdes0 = RDES0_OWN;
		rx_dma_desc[cnt].rdes1 = RDES1_RCH | RX_BUF_SIZE;
		rx_dma_desc[cnt].rdes2 = (uint32_t)rx_buff[cnt];
		rx_dma_desc[cnt].rdes3 = (uint32_t)&rx_dma_desc[(cnt + 1) % RX_BUFS_NUMBER];
	}
	curr_rx_dma_desc = rx_dma_desc;
	for (cnt = 0; cnt < TX_BUFS_NUMBER; cnt++)
	{
		tx_dma_desc[cnt].tdes0 = TDES0_TCH;
		tx_dma_desc[cnt].tdes1 = 0;
		tx_dma_desc[cnt].tdes2 = (uint32_t)tx_buff[cnt];
		tx_dma_desc[cnt].tdes3 = (uint32_t)&tx_dma_desc[(cnt + 1) % TX_BUFS_NUMBER];
	}
	curr_tx_dma_desc = tx_dma_desc;

	ETH->DMARDLAR = (uint32_t)rx_dma_desc;
	ETH->DMATDLAR = (uint32_t)tx_dma_desc;
	ETH->DMAIER = ETH_DMAIER_NISE | ETH_DMAIER_RIE | ETH_DMAIER_TIE;

	// enable the MAC reception and transmission
	ETH->MACCR |= ETH_MACCR_RE | ETH_MACCR_TE;
#if CHECKSUM_BY_HARDWARE
	// enable IPv4 checksum checking for received frame payloads' TCP/UDP/ICMP headers
	ETH->MACCR |= ETH_MACCR_IPCO;
#endif
	// flush transmit FIFO
	ETH->DMAOMR |= ETH_DMAOMR_FTF;
	while (ETH->DMAOMR & ETH_DMAOMR_FTF);
	// enable the DMA reception and transmission
	ETH->DMAOMR |= ETH_DMAOMR_SR | ETH_DMAOMR_ST;

	return mac_err_ok;
}

//--------------------------------------------
mac_error hal_lan8720_get_transmission_status(void)
{
#if 0
	// check for linked status here:
	if (not_linked)
	{
		return mac_err_not_linked;
	}
#endif

	// check that the descriptor is not owned by the DMA
	if (curr_tx_dma_desc->tdes0 & TDES0_OWN)
	{
		return mac_err_busy;
	}
	return mac_err_ok;
}

//--------------------------------------------
mac_error hal_lan8720_write_frame(const uint8_t *frame, uint16_t size)
{
	if ((tx_size + size) > TX_BUF_SIZE)
	{
		// too big size, start transmission and then continue
		return mac_err_tx_need_start;
	}
	memcpy((uint8_t *)(curr_tx_dma_desc->tdes2 + tx_size), frame, size);
	tx_size += size;
	return mac_err_ok;
}

//--------------------------------------------
void hal_lan8720_start_transmission(void)
{
	curr_tx_dma_desc->tdes1 = tx_size;
	curr_tx_dma_desc->tdes0 = TDES0_IC | TDES0_FS | TDES0_LS | TDES0_TCH;
#if CHECKSUM_BY_HARDWARE
	curr_tx_dma_desc->tdes0 |= TDES0_CIC_11;
#endif
	curr_tx_dma_desc->tdes0 |= TDES0_OWN;
	curr_tx_dma_desc = (tx_dma_desc_t *)(curr_tx_dma_desc->tdes3);
	tx_size = 0;
#if 0
	// does not work properly because of TBUS=0 but have to be TBUS=1
	if ((ETH->DMASR & ETH_DMASR_TBUS) != 0)
#else
	if ((ETH->DMASR & ETH_DMASR_TPS) == ETH_DMASR_TPS_Suspended)
#endif
	{
		ETH->DMASR = ETH_DMASR_TBUS;
		ETH->DMATPDR = 0;
	}
}

//--------------------------------------------
mac_error hal_lan8720_get_reception_status(uint16_t *size)
{
	rx_dma_desc_t *rx_desc;

	rx_desc = curr_rx_dma_desc;
	while (!(rx_desc->rdes0 & RDES0_OWN))
	{
		if (!(rx_desc->rdes0 & (RDES0_AFM | RDES0_ES)) &&
			 (rx_desc->rdes0 & RDES0_FS) &&
			 (rx_desc->rdes0 & RDES0_LS))
		{
			// valid frame, substruct 4 bytes of the CRC
			curr_rx_dma_desc = rx_desc;
			rx_size = (uint16_t)(((rx_desc->rdes0 & RDES0_FL_MASK) >> RDES0_FL_SHIFT) - 4);
			rx_offset = 0;
			*size = rx_size;
			return mac_err_ok;
		}
		// invalid frame, give descriptor back to DMA and continue
		rx_desc->rdes0 = RDES0_OWN;
		rx_desc = (rx_dma_desc_t *)rx_desc->rdes3;
	}
	curr_rx_dma_desc = rx_desc;
	return mac_err_rx_invalid;
}

//--------------------------------------------
size_t hal_lan8720_read_frame(uint8_t *frame, uint16_t size)
{
	if (size > rx_size - rx_offset)
	{
    	size = rx_size - rx_offset;
	}

	if (size > 0)
	{
		memcpy(frame, (uint8_t *)(curr_rx_dma_desc->rdes2) + rx_offset, size);
		rx_offset += size;
	}

	return size;
}

//--------------------------------------------
void hal_lan8720_release_reception_buffer(void)
{
	curr_rx_dma_desc->rdes0 = RDES0_OWN;
	curr_rx_dma_desc = (rx_dma_desc_t *)(curr_rx_dma_desc->rdes3);
	rx_size = 0;
	rx_offset = 0;
	if ((ETH->DMASR & ETH_DMASR_RBUS) != 0)
	{
		ETH->DMASR = ETH_DMASR_RBUS;
		ETH->DMATPDR = 0;
	}
}

//--------------------------------------------
__WEAK void eth_irq_rx_callback(void)
{
}

//--------------------------------------------
__WEAK void eth_irq_tx_callback(void)
{
}

//--------------------------------------------
// Ethernet interruption handler
void ETH_IRQHandler(void)
{
	uint32_t dmasr;

	dmasr = ETH->DMASR;
	// writing 1 to (unreserved) bits in ETH_DMASR register[16:0]
	// clears them and writing 0 has no effect.
	ETH->DMASR = dmasr;

	if (dmasr & ETH_DMASR_RS)
	{
		// frame reception is finished
		eth_irq_rx_callback();
	}
	if (dmasr & ETH_DMASR_TS)
	{
		// frame transmission is finished
		eth_irq_tx_callback();
	}
}
