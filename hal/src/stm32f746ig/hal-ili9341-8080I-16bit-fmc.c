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

//--------------------------------------------
// FMC(AHB3)
// GPIO_AF12_FMC
#define	PORT_D0         GPIO_D   // PD14 <-> DB00
#define	PIN_D0          14
#define	PORT_D1         GPIO_D   // PD15 <-> DB01
#define	PIN_D1          15
#define	PORT_D2         GPIO_D   // PD0  <-> DB02
#define	PIN_D2          0
#define	PORT_D3         GPIO_D   // PD1  <-> DB03
#define	PIN_D3          1
#define	PORT_D4         GPIO_E   // PE7  <-> DB04
#define	PIN_D4          7
#define	PORT_D5         GPIO_E   // PE8  <-> DB05
#define	PIN_D5          8
#define	PORT_D6         GPIO_E   // PE9  <-> DB06
#define	PIN_D6          9
#define	PORT_D7         GPIO_E   // PE10 <-> DB07
#define	PIN_D7          10
#define	PORT_D8         GPIO_E   // PE11 <-> DB08
#define	PIN_D8          11
#define	PORT_D9         GPIO_E   // PE12 <-> DB09
#define	PIN_D9          12
#define	PORT_D10        GPIO_E   // PE13 <-> DB10
#define	PIN_D10         13
#define	PORT_D11        GPIO_E   // PE14 <-> DB11
#define	PIN_D11         14
#define	PORT_D12        GPIO_E   // PE15 <-> DB12
#define	PIN_D12         15
#define	PORT_D13        GPIO_D   // PD8  <-> DB13
#define	PIN_D13         8
#define	PORT_D14        GPIO_D   // PD9  <-> DB14
#define	PIN_D14         9
#define	PORT_D15        GPIO_D   // PD10 <-> DB15
#define	PIN_D15         10
#define	PORT_NWE        GPIO_D   // PD5  --> WR
#define	PIN_NWE         5
#define	PORT_NOE        GPIO_D   // PD4  --> RD
#define	PIN_NOE         4
#define	PORT_NE4        GPIO_G   // PG12 --> CS_M
#define	PIN_NE4         12
#define	PORT_A6         GPIO_F   // PF12 --> RS
#define	PIN_A6          12
#define	PORT_BLK        GPIO_B   // PB15 --> BLK
#define	PIN_BLK         15


//--------------------------------------------
// Level 1 cache on STM32F7 Series and STM32H7 Series (AN4839)
// 3.1 STM32F7 and STM32H7 default settings
// By default, the MPU is disabled. In this case, the cache setting is defined as a default address map:
// Address range          | Memory region   | Memory type | Cache policy
// 0x60000000-0x7FFFFFFF    External RAM      Normal        WBWA
// 0xC0000000-0xDFFFFFFF    External Device   Device        -
// https://community.st.com/thread/31029
// The region at 0x60000000 is defined as memory region,
// so it can use caches and (most important) write buffer.
// Remap from 0x60000000 to 0xC0000000 is needed:
// SYSCFG->MEMRMP |= SYSCFG_MEMRMP_SWP_FMC_0


//--------------------------------------------
// Asynchronous static memories: Mode 2 - NOR Flash/PSRAM

#define FMC_BANK1_SUBBANK       4UL // FMC_NE4 is used
#define FMC_BCR        FMC_Bank1->BTCR[((FMC_BANK1_SUBBANK - 1) * 2)]
#define FMC_BTR        FMC_Bank1->BTCR[((FMC_BANK1_SUBBANK - 1) * 2 + 1)]
#define FMC_BWTR       FMC_Bank1E->BWTR[(FMC_BANK1_SUBBANK - 1)]
// HADDR[27:26] = FMC_BANK1_SUBBANK - 1
// Memory width  |  Data address issued to the memory
// 8-bit         |  HADDR[25:0]
// 16-bit        |  HADDR[25:1] >> 1
// 32-bit        |  HADDR[25:2] >> 1
#define ADDR_PIN_MASK   (0x1UL << (6 + 1)) // FMC_A6 is used
#define ADDR_REG        (0xC0000000UL + ((FMC_BANK1_SUBBANK - 1) << 26))                  // PIN_A6 = 0 (command)
#define ADDR_DATA       (0xC0000000UL + ((FMC_BANK1_SUBBANK - 1) << 26) + ADDR_PIN_MASK)  // PIN_A6 = 1 (data)

static uint16_t *addr;

#if 0
#define DMA_FMC_IRQ_PREEMPT_PRIORITY    0
#endif

//--------------------------------------------
void hal_ili9341_init(void)
{
	// Remap from 0x60000000 to 0xC0000000
	SYSCFG->MEMRMP |= SYSCFG_MEMRMP_SWP_FMC_0;

	// IO port B clock enable
	// IO port D clock enable
	// IO port E clock enable
	// IO port F clock enable
	// IO port G clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN;
	// FMC clock enable
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;

	hw_cfg_pin(GPIOx(PORT_D0),   PIN_D0,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D1),   PIN_D1,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D2),   PIN_D2,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D3),   PIN_D3,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D4),   PIN_D4,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D5),   PIN_D5,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D6),   PIN_D6,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D7),   PIN_D7,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D8),   PIN_D8,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D9),   PIN_D9,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D10),  PIN_D10,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D11),  PIN_D11,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D12),  PIN_D12,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D13),  PIN_D13,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D14),  PIN_D14,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_D15),  PIN_D15,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_NWE),  PIN_NWE,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_NOE),  PIN_NOE,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_NE4),  PIN_NE4,  GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_A6),   PIN_A6,   GPIOCFG_MODE_ALT | GPIO_AF12_FMC | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_BLK),  PIN_BLK,  GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	hw_set_pin(GPIOx(PORT_BLK), PIN_BLK, 1);

	// FMC_BCR:
	// MBKEN = 1: Corresponding memory bank is enabled
	// MUXEN = 0: Address/Data nonmultiplexed
	// MTYP = 10: NOR Flash/OneNAND Flash (Mode 2)
	// MWID = 01: 16 bits
	// FACCEN = 1: Corresponding NOR Flash memory access is enabled
	// BURSTEN = 0
	// WAITPOL = 0
	// WAITCFG = 0
	// WREN = 1: Write operations are enabled for the bank by the FMC
	// WAITEN = 0
	// EXTMOD = 0: Mode 2
	// ASYNCWAIT = 0
	// CPSIZE = 000
	// CBURSTRW = 0
	// CCLKEN = 0
	// WFDIS = 0
	FMC_BCR = FMC_BCR4_MBKEN | FMC_BCR4_MTYP_1 | FMC_BCR4_MWID_0 | FMC_BCR4_FACCEN | FMC_BCR4_WREN;

#if 1 /* CPU */
	// FMC_BTR:
	// 1 x HCLK ~= 4.6 ns (216MHz)
	// 18.3.1 Display Parallel 18/16/9/8-bit Interface Timing Characteristics (8080-I system):
	// Address setup time - 0 ns
	// ADDSET = 0000: 0 x HCLK clock cycle
	// Write Control pulse L duration - 15 ns
	// DATAST = 00000100: 4 x HCLK clock cycle
	// ACCMOD = 00: irrelevant
	FMC_BTR = FMC_BTR4_DATAST_2;
#endif

#if 0 /* DMA */
	// FMC_BTR:
	// 1 x HCLK ~= 4.6 ns (216MHz)
	// 18.3.1 Display Parallel 18/16/9/8-bit Interface Timing Characteristics (8080-I system):
	// Address setup time - 10 ns
	// ADDSET = 0010: 2 x HCLK clock cycle
	// Write Control pulse L duration - 75 ns
	// DATAST = 00010000: 16 x HCLK clock cycle
	// ACCMOD = 00: irrelevant
	FMC_BTR = FMC_BTR4_DATAST_4 | FMC_BTR4_ADDSET_1;
#endif

	// FMC_BWTR: irrelevant
}

//--------------------------------------------
void hal_ili9341_command(void)
{
	addr = (uint16_t *)ADDR_REG;
}

//--------------------------------------------
void hal_ili9341_data(void)
{
	addr = (uint16_t *)ADDR_DATA;
}

//--------------------------------------------
void hal_ili9341_tx(uint16_t data)
{
	*addr = data;
}

//--------------------------------------------
void *hal_ili9341_get_data_addr(void)
{
	return (void *)ADDR_DATA;
}

//--------------------------------------------
void hal_ili9341_init_dma(void)
{
	// DMA2 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// DMA stream disabled
	DMA2_Stream0->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream0->CR & DMA_SxCR_EN);

	DMA2_Stream0->CR =  DMA_SxCR_CHSEL_0  | // Channel selection: (001) channel 1
	                    DMA_SxCR_MBURST_0 | // Memory burst transfer configuration: (01) INCR4
	                    DMA_SxCR_PBURST_0 | // Peripheral burst transfer configuration: (01) INCR4
	                                        // Current target: (0) ignored
	                                        // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                        // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_0  | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_0  | // Peripheral data size: (01) 16-bit
						                    // Memory increment mode: (0) Memory address pointer is fixed
	                    DMA_SxCR_PINC     | // Peripheral increment mode: (1) Peripheral address pointer is incremented after each data transfer
	                                        // Circular mode: (0) disabled
	                    DMA_SxCR_DIR_1      // Data transfer direction: (10) Memory-to-memory
	                                      ; // Peripheral flow controller: (0) The DMA is the flow controller

	DMA2_Stream0->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 0
	// set dcmi dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_FMC_IRQ_PREEMPT_PRIORITY, 0));
	// enable dcmi dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
#endif
}

//--------------------------------------------
void hal_ili9341_write_dma(uint8_t *txbuf, uint32_t length)
{
	// DMA stream disabled
	DMA2_Stream0->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream0->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA2->LIFCR = DMA_LIFCR_CTCIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CFEIF0 | DMA_LIFCR_CHTIF0;

	// Set the DMA addresses
	DMA2_Stream0->PAR = (uint32_t)txbuf;
	DMA2_Stream0->M0AR = (uint32_t)ADDR_DATA;

	DMA2_Stream0->NDTR = length / sizeof(uint16_t);

	// Enable interrupts
	DMA2_Stream0->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA2_Stream0->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enabled
	DMA2_Stream0->CR |= DMA_SxCR_EN;
	while (!(DMA2_Stream0->CR & DMA_SxCR_EN));

	while ((DMA2_Stream0->CR & DMA_SxCR_EN));
}

#if 0
//--------------------------------------------
void DMA2_Stream0_IRQHandler(void)
{
	if (DMA2->LISR & DMA_LISR_TCIF0)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	}
	if (DMA2->LISR & DMA_LISR_HTIF0)
	{
		DMA2->LIFCR |= DMA_LIFCR_CHTIF0;
	}
	if (DMA2->LISR & DMA_LISR_TEIF0)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTEIF0;
	}
	if (DMA2->LISR & DMA_LISR_DMEIF0)
	{
		DMA2->LIFCR |= DMA_LIFCR_CDMEIF0;
	}
	if (DMA2->LISR & DMA_LISR_FEIF0)
	{
		DMA2->LIFCR |= DMA_LIFCR_CFEIF0;
	}
}
#endif

