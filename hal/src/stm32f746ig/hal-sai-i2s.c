/*
* Copyright (c) 2019 Vladimir Alemasov
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
// SAI1(PLLSAIQ, PLLI2SQ, I2S_CKIN)
// GPIO_AF6_SAI1
// MCK_A: PE2
// SCK_A: PE5
// SD_A:  PB2    PC1    PD6    PE6
// FS_A:  PE4
// MCK_B: PF7
// SCK_B: PF8
// SD_B:  PE3    PF6
// FS_B:  PF9
//
// SAI2(PLLSAIQ, PLLI2SQ, I2S_CKIN)
// GPIO_AF8_SAI2
// SCK_B: PA2
// FS_B:  PA12   PC0
// GPIO_AF10_SAI2
// MCK_A: PE0    PI4
// SCK_A: PD13   PI5
// SD_A:  PD11   PI6
// FS_A:  PD12   PI7
// MCK_B: PA1    PE6    PE14   PH3
// SCK_B: PE12   PH2
// SD_B:  PA0    PE11   PF11   PG10
// FS_B:  PE13   PG9
//
// GPIO_AF5_I2S_CKIN
// CKIN:  PC9
//
// The first clock source in the brackets
// is selected as SAI clock by default,
// see RCC->DKCFGR1 register

//--------------------------------------------
#define	PORT_CK         GPIO_I   // PI5  --> BCLK(BCK)
#define	PIN_CK          5
#define	PORT_WS         GPIO_I   // PI7  --> WSEL(LCK)
#define	PIN_WS          7
#define	PORT_SD         GPIO_I   // PI6  --> DIN
#define	PIN_SD          6
#define	PORT_MCLK       GPIO_I   // PI4  --> PA0(TIM2_ETR, see hal-i2s-mclk.c)
#define	PIN_MCLK        4

#if 1
#define DMA_SAI2_IRQ_PREEMPT_PRIORITY    0
#endif

//--------------------------------------------
#ifndef I2S_FCLK
// 44100, 48000, 88200, 96000, 176400, 192000 are allowed
#define I2S_FCLK       44100
#endif
#ifndef I2S_MCLK
#define I2S_MCLK       1
#endif
#ifndef I2S_BITRES
// 16, 24, 32 are allowed
#define I2S_BITRES     16
#endif
// For compatibility with the SPI-I2S interface, only 16 and 32-bit audio samples are used:
#if I2S_BITRES == 16
#define BYTES_PER_AUDIO_SAMPLE    2
#else
#define BYTES_PER_AUDIO_SAMPLE    4
#endif

//--------------------------------------------
// Frame clock (sound sampling rate) is applied to the WSEL(LCK) pin of the DAC(ADC)
// Sample clock is applied to the BCLK(BCK) pin of the DAC(ADC)
// Master clock can be used for USB feedback or/and the MCLK pin of the DAC(ADC)
//--------------------------------------------
// fFCLK - Frame clock
// fPLLSAIQ - SAI PLL clock
// fPLLI2SQ - I2S PLL clock
// fSCLK - Sample clock, fSCLK = fFCLK * 'bits per frame' * 2, where 2 -> two channels (stereo)
// fMCLK - Master clock, fMCLK = fFCLK * 256
// Base clocks in Hz:
// +--------+-------------+-------------------+-----------------------+----------+
// | fFCLK  |  fPLLSAIQ   |       fSCLK       |        fSCLK          |  fMCLK   |
// |        |  fPLLI2SQ   | 16-bit resolution | 32(24)-bit resolution |          |
// +--------+-------------+-------------------+-----------------------+----------+
// |  44100 | 90333333.33 | 1411200           | 2822400               | 11289600 |
// |  48000 | 98333333.33 | 1536000           | 3072000               | 12288000 |
// |  88200 | 90333333.33 | 2822400           | 5644800               | 22579200 |
// |  96000 | 98333333.33 | 3072000           | 6144000               | 24576000 |
// | 176400 | 90333333.33 | 5644800           | 11289600              | 45158400 |
// | 192000 | 98333333.33 | 6144000           | 12288000              | 49152000 |
// +--------+-------------+-------------------+-----------------------+----------+
//
//--------------------------------------------
// HSE(8000000Hz) -> fPLL M='/6' -> fPLLSAI N='x271' -> fPLLSAI Q='/4' -> fPLLSAIQ(90333333.33Hz)
#if I2S_FCLK == 44100 || I2S_FCLK == 88200 || I2S_FCLK == 176400
#define PLL_SAI_N      271
#define PLL_SAI_P      4 // not used here
#define PLL_SAI_Q      4
#define PLL_SAI_R      4 // not used here
#endif
//--------------------------------------------
// HSE(8000000Hz) -> fPLL M='/6' -> fPLLSAI N='x295' -> fPLLSAI Q='/4' -> fPLLSAIQ(98333333.33Hz)
#if I2S_FCLK == 48000 || I2S_FCLK == 96000 || I2S_FCLK == 192000
#define PLL_SAI_N      295
#define PLL_SAI_P      4 // not used here
#define PLL_SAI_Q      4
#define PLL_SAI_R      4 // not used here
#endif

//--------------------------------------------
static double i2s_mclk;

//--------------------------------------------
void hal_sai_i2s_init(void)
{
	// SAI clock from PLLSAIQ by default
	// configure the SAI PLL
	RCC->PLLSAICFGR = (PLL_SAI_N << RCC_PLLSAICFGR_PLLSAIN_Pos) | 
	                  (PLL_SAI_P << RCC_PLLSAICFGR_PLLSAIP_Pos) | 
	                  (PLL_SAI_Q << RCC_PLLSAICFGR_PLLSAIQ_Pos) | 
	                  (PLL_SAI_R << RCC_PLLSAICFGR_PLLSAIR_Pos);

	// PLLSAI enable
	RCC->CR |= RCC_CR_PLLSAION;

	// IO port I clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
	// SAI2 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_SAI2EN;

	hw_cfg_pin(GPIOx(PORT_CK), PIN_CK, GPIOCFG_MODE_ALT | GPIO_AF10_SAI2 | GPIOCFG_OSPEED_MEDIUM | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_WS), PIN_WS, GPIOCFG_MODE_ALT | GPIO_AF10_SAI2 | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_SD), PIN_SD, GPIOCFG_MODE_ALT | GPIO_AF10_SAI2 | GPIOCFG_OSPEED_MEDIUM | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
#if I2S_MCLK == 1
	hw_cfg_pin(GPIOx(PORT_MCLK), PIN_MCLK, GPIOCFG_MODE_ALT | GPIO_AF10_SAI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
#endif
}

//--------------------------------------------
void hal_sai_i2s_config(uint8_t master_transmit)
{
	double i2s_clk;
	uint32_t reg;

	// fPLLI2SR
	i2s_clk = (double)HSE_VALUE / 6.0 * (double)PLL_SAI_N / (double)PLL_SAI_Q;

	// RM0385 Rev 7 1157/1709:
	//-----------------------------
	// fMCLK = fPLLSAIQ / (MCKDIV*2)
	// fMCLK = fFCLK*256
	// Therefore:
	// MCKDIV = fPLLSAIQ / (fFCLK*2*256)
	// fFCLK = fPLLSAIQ / (MCKDIV*2*256)

	//-----------------------------
	// NODIV = 0: Master clock generator is enabled
	// DMAEN = 0: DMA disabled
	// SAIEN = 0: SAI audio block disabled
	// OUTDRIV = 0: Audio block output driven when SAIEN is set
	// MONO = 0: Stereo mode
	// SYNCEN[1:0] = 00: audio sub-block in asynchronous mode
	// CKSTR = 0: Signals generated by the SAI change on SCK raising edge
	// LSBFIRST = 0: Data are transferred with MSB first
	// PRTCFG[1:0] = 00: Free protocol
	reg = 0;
	if (master_transmit)
	{
		// MODE[1:0] = 00: Master transmitter
		reg |= SAI_xCR1_DS_2;
	}
	else
	{
		// MODE[1:0] = 01: Master receiver
		reg |= SAI_xCR1_DS_2 | SAI_xCR1_MODE_0;
	}
#if BYTES_PER_AUDIO_SAMPLE == 2
	// DS[2:0] = 100: Data size - 16 bits
	reg |= SAI_xCR1_DS_2;
#else
	// DS[2:0] = 111: Data size - 32 bits
	reg |= SAI_xCR1_DS_2 | SAI_xCR1_DS_1 | SAI_xCR1_DS_0;
#endif
#if I2S_FCLK == 44100 || I2S_FCLK == 48000
	// MCKDIV[3:0] = 4: Master clock divider
	reg |= (4 << SAI_xCR1_MCKDIV_Pos);
	// fFCLK = fPLLSAIQ / (MCKDIV*2*256)
	// Real Frame clock for ideal 44100 Hz:
	// fFCLK = 90333333.33 / (4*2*256) ~= 44108.073 Hz
	// Real Frame clock for ideal 48000 Hz:
	// fFCLK = 98333333.33 / (4*2*256) ~= 48014.323 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (4*2*256)) * 256;
#elif I2S_FCLK == 88200 || I2S_FCLK == 96000
	// MCKDIV[3:0] = 2: Master clock divider
	reg |= (2 << SAI_xCR1_MCKDIV_Pos);
	// fFCLK = fPLLSAIQ / (MCKDIV*2*256)
	// Real Frame clock for ideal 44100 Hz:
	// fFCLK = 90333333.33 / (2*2*256) ~= 44108.073 Hz
	// Real Frame clock for ideal 48000 Hz:
	// fFCLK = 98333333.33 / (2*2*256) ~= 48014.323 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (2*2*256)) * 256;
#elif I2S_FCLK == 176400 || I2S_FCLK == 192000
	// MCKDIV[3:0] = 1: Master clock divider
	reg |= (1 << SAI_xCR1_MCKDIV_Pos);
	// fFCLK = fPLLSAIQ / (MCKDIV*2*256)
	// Real Frame clock for ideal 44100 Hz:
	// fFCLK = 90333333.33 / (1*2*256) ~= 44108.073 Hz
	// Real Frame clock for ideal 48000 Hz:
	// fFCLK = 98333333.33 / (1*2*256) ~= 48014.323 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (1*2*256)) * 256;
#endif
	SAI2_Block_A->CR1 = reg;

	//-----------------------------
	// COMP[1:0] = 00: No companding algorithm
	// CPL: ignored
	// MUTECNT[5:0]: ignored
	// MUTEVAL = 0: Bit value 0 is sent during the mute mode
	// MUTE = 0: No mute mode
	// TRIS = 0: SD output line is still driven by the SAI when a slot is inactive
	// FFLUSH = 0: No FIFO flush
	// FTH[2:0] = 000: FIFO threshold: FIFO empty
	SAI2_Block_A->CR2 = 0; // reset value

	//-----------------------------
	// FSOFF = 1: FS is asserted one bit before the first bit of the slot 0
	// FSPOL = 0: FS is active low (falling edge)
	// FSDEF = 1: FS signal is a start of frame signal + channel side identification
#if BYTES_PER_AUDIO_SAMPLE == 2
	// FSALL[6:0] = 15: the number of bits in the active level of the audio frame - 1 = 16 - 1 = 15
	// FRL[7:0] = 31: the number of bits in the audio frame - 1 = 32 - 1 = 31
	SAI2_Block_A->FRCR = SAI_xFRCR_FSOFF | SAI_xFRCR_FSDEF | (15 << SAI_xFRCR_FSALL_Pos) | (31 << SAI_xFRCR_FRL_Pos);
#else
	// FSALL[6:0] = 31: the number of bits in the active level of the audio frame - 1 = 32 - 1 = 31
	// FRL[7:0] = 63: the number of bits in the audio frame - 1 = 64 - 1 = 63
	SAI2_Block_A->FRCR = SAI_xFRCR_FSOFF | SAI_xFRCR_FSDEF | (31 << SAI_xFRCR_FSALL_Pos) | (63 << SAI_xFRCR_FRL_Pos);
#endif

	//-----------------------------
	// SLOTEN[15:0] = 0xFFFF: All slots are enable
	// NBSLOT[3:0] = 1: Number of slots in an audio frame - 1 = 2 - 1 = 1
	// FBOFF[4:0] = 0000: First bit offset
#if BYTES_PER_AUDIO_SAMPLE == 2
	// SLOTSZ[1:0] = 01: 16-bit
	SAI2_Block_A->SLOTR = (0xFFFFU << SAI_xSLOTR_SLOTEN_Pos) | SAI_xSLOTR_NBSLOT_0 | SAI_xSLOTR_SLOTSZ_0;
#else
	// SLOTSZ[1:0] = 10: 32-bit
	SAI2_Block_A->SLOTR = (0xFFFFU << SAI_xSLOTR_SLOTEN_Pos) | SAI_xSLOTR_NBSLOT_0 | SAI_xSLOTR_SLOTSZ_1;
#endif

	// SAI enable
	SAI2_Block_A->CR1 |= SAI_xCR1_SAIEN;
}

//--------------------------------------------
double hal_sai_i2s_get_mclk(void)
{
	return i2s_mclk;
}

//--------------------------------------------
void hal_sai_i2s_init_dma_tx_buf(void)
{
	// DMA2 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// DMA stream disable
	DMA2_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream4->CR & DMA_SxCR_EN);

	DMA2_Stream4->CR =  DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1 | // Channel selection: (011) channel 3
	                                       // Memory burst transfer configuration: (00) single transfer
	                                       // Peripheral burst transfer configuration: (00) single transfer
	                                       // Current target: (0) ignored
	                                       // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                       // Peripheral increment offset size: (0) ignored
#if BYTES_PER_AUDIO_SAMPLE == 2
	                    DMA_SxCR_MSIZE_0 | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_0 | // Peripheral data size: (01) 16-bit
#else
						DMA_SxCR_MSIZE_1 | // Memory data size: (10) 32-bit
						DMA_SxCR_PSIZE_1 | // Peripheral data size: (10) 32-bit
#endif
	                    DMA_SxCR_MINC |    // Memory increment mode: (1) incremented after each data transfer
	                                       // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                                       // Circular mode: (0) disabled
	                    DMA_SxCR_DIR_0     // Data transfer direction: (01) Memory-to-peripheral
	                                     ; // Peripheral flow controller: (0) DMA is the flow controller

	DMA2_Stream4->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 1
	// set sai2 dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_SAI2_IRQ_PREEMPT_PRIORITY, 0));
	// enable sai2 dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream4_IRQn);
#endif
}

//--------------------------------------------
void hal_sai_i2s_init_dma_rx_buf(void)
{
	// DMA2 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// DMA stream disable
	DMA2_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream4->CR & DMA_SxCR_EN);

	DMA2_Stream4->CR =  DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1 | // Channel selection: (011) channel 3
	                                       // Memory burst transfer configuration: (00) single transfer
	                                       // Peripheral burst transfer configuration: (00) single transfer
	                                       // Current target: (0) ignored
	                                       // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                       // Peripheral increment offset size: (0) ignored
#if BYTES_PER_AUDIO_SAMPLE == 2
	                    DMA_SxCR_MSIZE_0 | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_0 | // Peripheral data size: (01) 16-bit
#else
						DMA_SxCR_MSIZE_1 | // Memory data size: (10) 32-bit
						DMA_SxCR_PSIZE_1 | // Peripheral data size: (10) 32-bit
#endif
	                    DMA_SxCR_MINC      // Memory increment mode: (1) incremented after each data transfer
	                                       // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                                       // Circular mode: (0) disabled
	                                       // Data transfer direction: (00) Peripheral-to-memory
	                                     ; // Peripheral flow controller: (0) DMA is the flow controller

	DMA2_Stream4->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 1
	// set sai2 dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_SAI2_IRQ_PREEMPT_PRIORITY, 0));
	// enable sai2 dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream4_IRQn);
#endif
}

//--------------------------------------------
void hal_sai_i2s_init_dma_tx_buf_cycle(void)
{
	// DMA2 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// DMA stream disabled
	DMA2_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream4->CR & DMA_SxCR_EN);

	DMA2_Stream4->CR =  DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1 | // Channel selection: (011) channel 3
	                                       // Memory burst transfer configuration: (00) single transfer
	                                       // Peripheral burst transfer configuration: (00) single transfer
	                                       // Current target: (0) ignored
	                                       // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                       // Peripheral increment offset size: (0) ignored
#if BYTES_PER_AUDIO_SAMPLE == 2
	                    DMA_SxCR_MSIZE_0 | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_0 | // Peripheral data size: (01) 16-bit
#else
						DMA_SxCR_MSIZE_1 | // Memory data size: (10) 32-bit
						DMA_SxCR_PSIZE_1 | // Peripheral data size: (10) 32-bit
#endif
	                    DMA_SxCR_MINC |    // Memory increment mode: (1) incremented after each data transfer
	                                       // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                    DMA_SxCR_CIRC |    // Circular mode: (1) enabled
	                    DMA_SxCR_DIR_0     // Data transfer direction: (01) Memory-to-peripheral
	                                     ; // Peripheral flow controller: (0) DMA is the flow controller

	DMA2_Stream4->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 1
	// set sai2 dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_SAI2_IRQ_PREEMPT_PRIORITY, 0));
	// enable sai2 dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream4_IRQn);
#endif
}

//--------------------------------------------
void hal_sai_i2s_write_dma_buf(void *txbuf, uint32_t length)
{
	// SAI2 DMA enable
	SAI2_Block_A->CR1 |= SAI_xCR1_DMAEN;

	// DMA stream disabled
	DMA2_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream4->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA2->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4 | DMA_HIFCR_CHTIF4;

	// Set the DMA addresses
	DMA2_Stream4->PAR = (uint32_t)&(SAI2_Block_A->DR);
	DMA2_Stream4->M0AR = (uint32_t)txbuf;

#if BYTES_PER_AUDIO_SAMPLE == 2
	// Set the number of 16-bit words to transfer
	DMA2_Stream4->NDTR = length / sizeof(uint16_t);
#else
	// Set the number of 32-bit words to transfer
	DMA2_Stream4->NDTR = length / sizeof(uint32_t);
#endif

	// Enable interrupts
	DMA2_Stream4->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA2_Stream4->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enabled
	DMA2_Stream4->CR |= DMA_SxCR_EN;
	while (!(DMA2_Stream4->CR & DMA_SxCR_EN));
}

//--------------------------------------------
void hal_sai_i2s_write_dma_buf_stop(void *txbuf, uint32_t length, uint32_t timeout)
{
	uint32_t start;

	// SAI2 DMA enable
	SAI2_Block_A->CR1 |= SAI_xCR1_DMAEN;

	// DMA stream disable
	DMA2_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream4->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA2->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4 | DMA_HIFCR_CHTIF4;

	// Set the DMA addresses
	DMA2_Stream4->PAR = (uint32_t)&(SAI2_Block_A->DR);
	DMA2_Stream4->M0AR = (uint32_t)txbuf;

#if BYTES_PER_AUDIO_SAMPLE == 2
	// Set the number of 16-bit words to transfer
	DMA2_Stream4->NDTR = length / sizeof(uint16_t);
#else
	// Set the number of 32-bit words to transfer
	DMA2_Stream4->NDTR = length / sizeof(uint32_t);
#endif

	// Enable interrupts
	DMA2_Stream4->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA2_Stream4->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enable
	DMA2_Stream4->CR |= DMA_SxCR_EN;
	while (!(DMA2_Stream4->CR & DMA_SxCR_EN));

	for (start = get_platform_counter();;)
	{
		if ((get_platform_counter() - start) >= timeout)
		{
			DMA2_Stream4->CR &= ~DMA_SxCR_EN;
			while (DMA2_Stream4->CR & DMA_SxCR_EN);
		}
		if (!(DMA2_Stream4->CR & DMA_SxCR_EN))
		{
			// SAI2 DMA disable
			SAI2_Block_A->CR1 &= ~SAI_xCR1_DMAEN;
			return;
		}
	}
}

//--------------------------------------------
void hal_sai_i2s_stop_dma(void)
{
	// DMA stream disabled
	DMA2_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream4->CR & DMA_SxCR_EN);
	// SAI2 DMA disable
	SAI2_Block_A->CR1 &= ~SAI_xCR1_DMAEN;
}


#if 1
//--------------------------------------------
__WEAK void hal_sai_i2s_irq_htif_callback(void)
{
}

//--------------------------------------------
__WEAK void hal_sai_i2s_irq_tcif_callback(void)
{
}

//--------------------------------------------
void DMA2_Stream4_IRQHandler(void)
{
	if (DMA2->HISR & DMA_HISR_TCIF4)
	{
		DMA2->HIFCR |= DMA_HIFCR_CTCIF4;
		hal_sai_i2s_irq_tcif_callback();
	}
	if (DMA2->HISR & DMA_HISR_HTIF4)
	{
		DMA2->HIFCR |= DMA_HIFCR_CHTIF4;
		hal_sai_i2s_irq_htif_callback();
	}
	if (DMA2->HISR & DMA_HISR_TEIF4)
	{
		DMA2->HIFCR |= DMA_HIFCR_CTEIF4;
	}
	if (DMA2->HISR & DMA_HISR_DMEIF4)
	{
		DMA2->HIFCR |= DMA_HIFCR_CDMEIF4;
	}
	if (DMA2->HISR & DMA_HISR_FEIF4)
	{
		DMA2->HIFCR |= DMA_HIFCR_CFEIF4;
	}
}
#endif
