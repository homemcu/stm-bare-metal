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

//--------------------------------------------
// SPI1(APB2)
// GPIO_AF5_SPI1
// SCK:   PA5    PB3
// MISO:  PA6    PB4
// MOSI:  PA7    PB5
//
// SPI2(APB1)
// GPIO_AF5_SPI2
// SCK:   PB10   PB13    PI0
// MISO:  PC2    PB14    PI2
// MOSI:  PC3    PB15    PI3
//
// SPI3(APB1)
// GPIO_AF6_SPI3
// SCK:   PB3    PC10
// MISO:  PB4    PC11
// MOSI:  PB5    PC12

//--------------------------------------------
#define	PORT_SCK        GPIO_A   // PA5 --> SCK
#define	PIN_SCK         5
#define	PORT_MOSI       GPIO_B   // PB5 --> SDI(MOSI)
#define	PIN_MOSI        5
#define	PORT_RESET      GPIO_E   // PE2 --> RESET
#define	PIN_RESET       2
#define	PORT_CS         GPIO_E   // PE3 --> CS
#define	PIN_CS          3
#define	PORT_DC         GPIO_F   // PF0 --> DC
#define	PIN_DC          0

//--------------------------------------------
// SPI Data Transfer Frequency (10MHz max)
// SPI1_CK = PCLK2(84MHz) / 16 = 5.25MHz
#define SPI_CLK_DIV      (SPI_CR1_BR_1 | SPI_CR1_BR_0)

#if 1
#define DMA_SPI1_IRQ_PREEMPT_PRIORITY    0
#endif

//--------------------------------------------
void hal_ili9341_init(void)
{
	// IO port A clock enable
	// IO port B clock enable
	// IO port E clock enable
	// IO port F clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN;
	// SPI1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// Set first to prevent glitches
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);			// PIN_CS = 1
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);	// PIN_RESET = 1

	hw_cfg_pin(GPIOx(PORT_SCK),    PIN_SCK,    GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_MOSI),   PIN_MOSI,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_RESET),  PIN_RESET,  GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_CS),     PIN_CS,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_DC),     PIN_DC,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	// BIDIMODE = 1: 1-line bidirectional data mode selected
	// BIDIOE = 1: Output enabled (transmit-only mode)
	// SSM = 1: Software slave management enabled
	// SSI = 1: Internal slave select in master mode
	// SPE = 1: SPI enable
	// BR[2:0]: Baud rate control
	// MSTR = 1: Master configuration
	SPI1->CR1 = SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CLK_DIV | SPI_CR1_MSTR;
}

//--------------------------------------------
void hal_ili9341_reset(void)
{
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 0);	// PIN_RESET = 0
	delay_ms(1);
	hw_set_pin(GPIOx(PORT_RESET), PIN_RESET, 1);	// PIN_RESET = 1
	delay_ms(2);
}

//--------------------------------------------
void hal_ili9341_select(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 0);			// PIN_CS = 0
}

//--------------------------------------------
void hal_ili9341_release(void)
{
	hw_set_pin(GPIOx(PORT_CS), PIN_CS, 1);			// PIN_CS = 1
}

//--------------------------------------------
void hal_ili9341_command(void)
{
	hw_set_pin(GPIOx(PORT_DC), PIN_DC, 0);			// PIN_DC = 0
}

//--------------------------------------------
void hal_ili9341_data(void)
{
	hw_set_pin(GPIOx(PORT_DC), PIN_DC, 1);			// PIN_DC = 1
}

//--------------------------------------------
void hal_ili9341_tx(uint8_t data)
{
	while (!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = data;
}

//--------------------------------------------
void hal_ili9341_tx_complete(void)
{
	while (!(SPI1->SR & SPI_SR_TXE));
	while ((SPI1->SR & SPI_SR_BSY));
}

//--------------------------------------------
void hal_ili9341_init_dma(void)
{
	// DMA2 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// DMA stream disabled
	DMA2_Stream5->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream5->CR & DMA_SxCR_EN);

	DMA2_Stream5->CR =  DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1 | // Channel selection: (011) channel 3
	                                        // Memory burst transfer configuration: (00) single transfer
	                                        // Peripheral burst transfer configuration: (00) single transfer
	                                        // Current target: (0) ignored
	                                        // Double buffer mode: (0) No buffer switching at the end of transfer
	                                        // Priority level: (00) Low
	                                        // Peripheral increment offset size: (0) ignored
	                                        // Memory data size: (00) 8-bit
	                                        // Peripheral data size: (00) 8-bit
	                    DMA_SxCR_MINC     | // Memory increment mode: (1) incremented after each data transfer
	                                        // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                                        // Circular mode: (0) disabled
	                    DMA_SxCR_DIR_0      // Data transfer direction: (01) Memory-to-peripheral
	                                      ; // Peripheral flow controller: (0) DMA is the flow controller

	DMA2_Stream5->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 1
	// set spi1 dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_SPI1_IRQ_PREEMPT_PRIORITY, 0));
	// enable spi1 dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream5_IRQn);
#endif
}

//--------------------------------------------
void hal_ili9341_write_dma(uint8_t *txbuf, uint32_t length)
{
	// SPI1 DMA enable
	SPI1->CR2 |= SPI_CR2_TXDMAEN;

	// DMA stream disabled
	DMA2_Stream5->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream5->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA2->HIFCR = DMA_HIFCR_CTCIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5 | DMA_HIFCR_CHTIF5;

	// Set the DMA addresses
	DMA2_Stream5->PAR = (uint32_t)&(SPI1->DR);
	DMA2_Stream5->M0AR = (uint32_t)txbuf;

	// Set the number of 8-bit words to transfer
	DMA2_Stream5->NDTR = length;

	// Enable interrupts
	DMA2_Stream5->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA2_Stream5->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enabled
	DMA2_Stream5->CR |= DMA_SxCR_EN;
	while (!(DMA2_Stream5->CR & DMA_SxCR_EN));

	while ((DMA2_Stream5->CR & DMA_SxCR_EN));
	while ((SPI1->SR & SPI_SR_BSY));

	SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
}

#if 1
//--------------------------------------------
void DMA2_Stream5_IRQHandler(void)
{
	if (DMA2->HISR & DMA_HISR_TCIF5)
	{
		DMA2->HIFCR |= DMA_HIFCR_CTCIF5;
	}
	if (DMA2->HISR & DMA_HISR_HTIF5)
	{
		DMA2->HIFCR |= DMA_HIFCR_CHTIF5;
	}
	if (DMA2->HISR & DMA_HISR_TEIF5)
	{
		DMA2->HIFCR |= DMA_HIFCR_CTEIF5;
	}
	if (DMA2->HISR & DMA_HISR_DMEIF5)
	{
		DMA2->HIFCR |= DMA_HIFCR_CDMEIF5;
	}
	if (DMA2->HISR & DMA_HISR_FEIF5)
	{
		DMA2->HIFCR |= DMA_HIFCR_CFEIF5;
	}
}
#endif
