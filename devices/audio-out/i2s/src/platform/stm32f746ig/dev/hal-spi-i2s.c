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
// I2S1(PLLI2SR, I2S_CKIN)
// GPIO_AF5_I2S1
// CK:    PA5    PB3
// WS:    PA4    PA15
// SD:    PA7    PB5
// MCK:   PC4
//
// I2S2(PLLI2SR, I2S_CKIN)
// GPIO_AF5_I2S2
// CK:    PA9    PB10   PB13    PD3    PI1
// WS:    PB9    PB12   PI0
// SD:    PB15   PC1    PC3     PI3
// MCK:   PC6
//
// I2S3(PLLI2SR, I2S_CKIN)
// GPIO_AF5_I2S3
// SD:    PD6
// GPIO_AF6_I2S3
// CK:    PB3    PC10
// WS:    PA4    PA15
// SD:    PB5    PC12
// MCK:   PC7
//
// GPIO_AF5_I2S_CKIN
// CKIN:  PC9
//
// The first clock source in the brackets
// is selected as I2S clock by default,
// see RCC->CFGR register

//--------------------------------------------
#define	PORT_CK         GPIO_I   // PI1  --> BCLK(BCK)
#define	PIN_CK          1
#define	PORT_WS         GPIO_I   // PI0  --> WSEL(LCK)
#define	PIN_WS          0
#define	PORT_SD         GPIO_I   // PI3  --> DIN
#define	PIN_SD          3
#define	PORT_MCLK       GPIO_C   // PC6
#define	PIN_MCLK        6

//--------------------------------------------
#if 1
#define DMA_SPI2_IRQ_PREEMPT_PRIORITY    0
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
// Based on the specificity of the SPI-I2S interface, only 16-bit and 32-bit variants may be used:
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
// fPLLI2SR - I2S PLL clock
// fSCLK - Sample clock, fSCLK = fFCLK * 'bits per frame' * 2, where 2 -> two channels (stereo)
// fMCLK - Master clock, fMCLK = fFCLK * 256
// Base clocks in Hz:
// +--------+-------------+-------------------+-----------------------+----------+
// | fFCLK  |  fPLLI2SR   |       fSCLK       |        fSCLK          |  fMCLK   |
// |        |             | 16-bit resolution | 32(24)-bit resolution |          |
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
// HSE(8000000Hz) -> fPLL M='/6' -> fPLLI2S N='x271' -> fPLLI2S R='/4' -> fPLLI2SR(90333333.33Hz)
#if I2S_FCLK == 44100 || I2S_FCLK == 88200 || I2S_FCLK == 176400
#define PLL_I2S_N      271
#define PLL_I2S_P      2 // not used here
#define PLL_I2S_Q      4 // not used here
#define PLL_I2S_R      4
#endif
//--------------------------------------------
// HSE(8000000Hz) -> fPLL M='/6' -> fPLLI2S N='x295' -> fPLLI2S R='/4' -> fPLLI2SR(98333333.33Hz)
#if I2S_FCLK == 48000 || I2S_FCLK == 96000 || I2S_FCLK == 192000
#define PLL_I2S_N      295
#define PLL_I2S_P      2 // not used here
#define PLL_I2S_Q      4 // not used here
#define PLL_I2S_R      4
#endif

//--------------------------------------------
static double i2s_mclk;

//--------------------------------------------
void hal_spi_i2s_init(void)
{
	// I2S clock from PLLI2SR by default
	// configure the I2S PLL
	RCC->PLLI2SCFGR = (PLL_I2S_N << RCC_PLLI2SCFGR_PLLI2SN_Pos) | 
	                  (PLL_I2S_P << RCC_PLLI2SCFGR_PLLI2SP_Pos) | 
	                  (PLL_I2S_Q << RCC_PLLI2SCFGR_PLLI2SQ_Pos) | 
	                  (PLL_I2S_R << RCC_PLLI2SCFGR_PLLI2SR_Pos);

	// PLLI2S enable
	RCC->CR |= RCC_CR_PLLI2SON;

	// IO port I clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
#if I2S_MCLK == 1
	// IO port C clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
#endif

	// SPI2 clock enable
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

	hw_cfg_pin(GPIOx(PORT_CK), PIN_CK, GPIOCFG_MODE_ALT | GPIO_AF5_I2S2 | GPIOCFG_OSPEED_MEDIUM | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_WS), PIN_WS, GPIOCFG_MODE_ALT | GPIO_AF5_I2S2 | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_SD), PIN_SD, GPIOCFG_MODE_ALT | GPIO_AF5_I2S2 | GPIOCFG_OSPEED_MEDIUM | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
#if I2S_MCLK == 1
	hw_cfg_pin(GPIOx(PORT_MCLK), PIN_MCLK, GPIOCFG_MODE_ALT | GPIO_AF5_I2S2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
#endif
}

//--------------------------------------------
void hal_spi_i2s_config(uint8_t master_transmit)
{
	double i2s_clk;
	uint32_t reg;

	// fPLLI2SR
	i2s_clk = (double)HSE_VALUE / 6.0 * (double)PLL_I2S_N / (double)PLL_I2S_R;

	// RM0385 Rev 7 1124/1709:
	//-----------------------------
	// When the master clock is enabled (MCKOE bit is set):
	// fFCLK = fPLLI2SR / (256*((2*I2SDIV) + ODD))
	// So when ODD=0 (ODD bit cleared)
	// I2SDIV = fPLLI2SR / (fFCLK*256*2)
	//-----------------------------
	// When the master clock is disabled (MCKOE bit cleared):
	// fFCLK = fPLLI2SR / (32*(CHLEN+1)*((2*I2SDIV) + ODD))
	// CHLEN = 0 when the channel frame is 16-bit wide
	// CHLEN = 1 when the channel frame is 32-bit wide
	// So when ODD=0 (ODD bit cleared) and CHLEN=0 (16-bit)
	// I2SDIV = fPLLI2SR / (fFCLK*32*1*2)
	// So when ODD=0 (ODD bit cleared) and CHLEN=1 (32-bit)
	// I2SDIV = fPLLI2SR / (fFCLK*32*2*2)

	//-----------------------------
#if I2S_MCLK == 1
#if I2S_FCLK == 44100 || I2S_FCLK == 48000
	// MCKOE = 1: Master clock output is enabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x04:  I2S linear prescaler
	SPI2->I2SPR = (0x04 << SPI_I2SPR_I2SDIV_Pos) | SPI_I2SPR_MCKOE;
	// fFCLK = fPLLI2SR / (256*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 44100 Hz:
	// fFCLK = 90333333.33 / (256*((2*4) + 0)) ~= 44108.073 Hz
	// Real Frame clock for ideal 48000 Hz:
	// fFCLK = 98333333.33 / (256*((2*4) + 0)) ~= 48014.323 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (256*((2*4) + 0))) * 256;
#elif I2S_FCLK == 88200 || I2S_FCLK == 96000
	// MCKOE = 1: Master clock output is enabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x02:  I2S linear prescaler
	SPI2->I2SPR = (0x02 << SPI_I2SPR_I2SDIV_Pos) | SPI_I2SPR_MCKOE;
	// fFCLK = fPLLI2SR / (256*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 88200 Hz:
	// fFCLK = 90333333.33 / (256*((2*2) + 0)) ~= 88216.146 Hz
	// Real Frame clock for ideal 96000 Hz:
	// fFCLK = 98333333.33 / (256*((2*2) + 0)) ~= 96028.646 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (256*((2*2) + 0))) * 256;
#elif I2S_FCLK == 176400 || I2S_FCLK == 192000
	// MCKOE = 1: Master clock output is enabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x01:  I2S linear prescaler
	SPI2->I2SPR = (0x01 << SPI_I2SPR_I2SDIV_Pos) | SPI_I2SPR_MCKOE;
	// fFCLK = fPLLI2SR / (256*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 176400 Hz:
	// fFCLK = 90333333.33 / (256*((2*1) + 0)) ~= 176432.292 Hz
	// Real Frame clock for ideal 192000 Hz:
	// fFCLK = 98333333.33 / (256*((2*1) + 0)) ~= 192057.292 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (256*((2*1) + 0))) * 256;
#endif
#else
#if I2S_FCLK == 44100 || I2S_FCLK == 48000
#if BYTES_PER_AUDIO_SAMPLE == 2
	// MCKOE = 1: Master clock output is disabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x20:  I2S linear prescaler
	// CHLEN = 0: 16 bits per audio channel
	SPI2->I2SPR = (0x20 << SPI_I2SPR_I2SDIV_Pos);
	// fFCLK = fPLLI2SR / (32*(CHLEN+1)*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 44100 Hz:
	// fFCLK = 90333333.33 / (32*(0+1)*((2*32) + 0)) ~= 44108.073 Hz
	// Real Frame clock for ideal 48000 Hz:
	// fFCLK = 98333333.33 / (32*(0+1)*((2*32) + 0)) ~= 48014.323 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (32*(0+1)*((2*32) + 0))) * 256;
#else
	// MCKOE = 1: Master clock output is disabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x20:  I2S linear prescaler
	// CHLEN = 1: 32 bits per audio channel
	SPI2->I2SPR = (0x10 << SPI_I2SPR_I2SDIV_Pos);
	// fFCLK = fPLLI2SR / (32*(CHLEN+1)*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 44100 Hz:
	// fFCLK = 90333333.33 / (32*(1+1)*((2*16) + 0)) ~= 44108.073 Hz
	// Real Frame clock for ideal 48000 Hz:
	// fFCLK = 98333333.33 / (32*(1+1)*((2*16) + 0)) ~= 48014.323 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (32*(1+1)*((2*16) + 0))) * 256;
#endif
#elif I2S_FCLK == 88200 || I2S_FCLK == 96000
#if BYTES_PER_AUDIO_SAMPLE == 2
	// MCKOE = 1: Master clock output is disabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x10:  I2S linear prescaler
	// CHLEN = 0: 16 bits per audio channel
	SPI2->I2SPR = (0x10 << SPI_I2SPR_I2SDIV_Pos);
	// fFCLK = fPLLI2SR / (32*(CHLEN+1)*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 88200 Hz:
	// fFCLK = 90333333.33 / (32*(0+1)*((2*16) + 0)) ~= 88216.146 Hz
	// Real Frame clock for ideal 96000 Hz:
	// fFCLK = 98333333.33 / (32*(0+1)*((2*16) + 0)) ~= 96028.646 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (32*(0+1)*((2*16) + 0))) * 256;
#else
	// MCKOE = 1: Master clock output is disabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x10:  I2S linear prescaler
	// CHLEN = 1: 32 bits per audio channel
	SPI2->I2SPR = (0x08 << SPI_I2SPR_I2SDIV_Pos);
	// fFCLK = fPLLI2SR / (32*(CHLEN+1)*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 88200 Hz:
	// fFCLK = 90333333.33 / (32*(1+1)*((2*8) + 0)) ~= 88216.146 Hz
	// Real Frame clock for ideal 96000 Hz:
	// fFCLK = 98333333.33 / (32*(1+1)*((2*8) + 0)) ~= 96028.646 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (32*(1+1)*((2*8) + 0))) * 256;
#endif
#elif I2S_FCLK == 176400 || I2S_FCLK == 192000
#if BYTES_PER_AUDIO_SAMPLE == 2
	// MCKOE = 1: Master clock output is disabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x08:  I2S linear prescaler
	// CHLEN = 0: 16 bits per audio channel
	SPI2->I2SPR = (0x08 << SPI_I2SPR_I2SDIV_Pos);
	// fFCLK = fPLLI2SR / (32*(CHLEN+1)*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 176400 Hz:
	// fFCLK = 90333333.33 / (32*(0+1)*((2*8) + 0)) ~= 176432.292 Hz
	// Real Frame clock for ideal 192000 Hz:
	// fFCLK = 98333333.33 / (32*(0+1)*((2*8) + 0)) ~= 192057.292 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (32*(0+1)*((2*8) + 0))) * 256;
#else
	// MCKOE = 1: Master clock output is disabled
	// ODD = 0: Real divider value is = I2SDIV*2
	// I2SDIV[7:0] = 0x08:  I2S linear prescaler
	// CHLEN = 1: 32 bits per audio channel
	SPI2->I2SPR = (0x04 << SPI_I2SPR_I2SDIV_Pos);
	// fFCLK = fPLLI2SR / (32*(CHLEN+1)*((2*I2SDIV) + ODD))
	// Real Frame clock for ideal 176400 Hz:
	// fFCLK = 90333333.33 / (32*(1+1)*((2*4) + 0)) ~= 176432.292 Hz
	// Real Frame clock for ideal 192000 Hz:
	// fFCLK = 98333333.33 / (32*(1+1)*((2*4) + 0)) ~= 192057.292 Hz
	// fMCLK
	i2s_mclk = (i2s_clk / (32*(1+1)*((2*4) + 0))) * 256;
#endif
#endif
#endif

	//-----------------------------
	// I2SMOD = 1: I2S mode is selected
	// I2SE = 1: I2S peripheral is enabled
	// CKPOL = 0(??): Sample clock inactive state is low level
	reg = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SE;
	if (master_transmit)
	{
		// I2SCFG[9:8] = 10: I2S configuration mode "Master - transmit"
		reg |= SPI_I2SCFGR_I2SCFG_1;
	}
	else
	{
		// I2SCFG[9:8] = 11: I2S configuration mode "Master - receive"
		reg |= SPI_I2SCFGR_I2SCFG_1 | SPI_I2SCFGR_I2SCFG_0;
	}
#if I2S_BITRES == 16
	// DATLEN[2:1] = 00: 16-bit data length
	// CHLEN = 0: 16 bits per audio channel
#elif I2S_BITRES == 24
	// DATLEN[2:1] = 01: 24-bit data length
	// CHLEN = 1: 32 bits per audio channel
	reg |= SPI_I2SCFGR_DATLEN_0 | SPI_I2SCFGR_CHLEN;
#elif I2S_BITRES == 32
	// DATLEN[2:1] = 10: 32-bit data length
	// CHLEN = 1: 32 bits per audio channel
	reg |= SPI_I2SCFGR_DATLEN_1 | SPI_I2SCFGR_CHLEN;
#endif
	SPI2->I2SCFGR = reg;
}

//--------------------------------------------
// Full reset, see comment to hal_i2s_stop_dma_tx() function
void hal_spi_i2s_reset(void)
{
	RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;
}

//--------------------------------------------
double hal_spi_i2s_get_mclk(void)
{
	return i2s_mclk;
}

//--------------------------------------------
void hal_spi_i2s_init_dma_tx_buf(void)
{
	// DMA1 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	// DMA stream disable
	DMA1_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream4->CR & DMA_SxCR_EN);

	DMA1_Stream4->CR =                     // Channel selection: (000) channel 0
	                                       // Memory burst transfer configuration: (00) single transfer
	                                       // Peripheral burst transfer configuration: (00) single transfer
	                                       // Current target: (0) ignored
	                                       // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                       // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_0 | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_0 | // Peripheral data size: (01) 16-bit
	                    DMA_SxCR_MINC |    // Memory increment mode: (1) incremented after each data transfer
	                                       // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                                       // Circular mode: (0) disabled
	                    DMA_SxCR_DIR_0     // Data transfer direction: (01) Memory-to-peripheral
	                                     ; // Peripheral flow controller: (0) DMA is the flow controller

	DMA1_Stream4->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 1
	// set spi2 dma global interrupt priority
	NVIC_SetPriority(DMA1_Stream4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_SPI2_IRQ_PREEMPT_PRIORITY, 0));
	// enable spi2 dma global interrupt
	NVIC_EnableIRQ(DMA1_Stream4_IRQn);
#endif
}

//--------------------------------------------
void hal_spi_i2s_init_dma_rx_buf(void)
{
	// DMA1 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	// DMA stream disable
	DMA1_Stream3->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream3->CR & DMA_SxCR_EN);

	DMA1_Stream3->CR =                     // Channel selection: (000) channel 0
	                                       // Memory burst transfer configuration: (00) single transfer
	                                       // Peripheral burst transfer configuration: (00) single transfer
	                                       // Current target: (0) ignored
	                                       // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                       // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_0 | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_0 | // Peripheral data size: (01) 16-bit
	                    DMA_SxCR_MINC      // Memory increment mode: (1) incremented after each data transfer
	                                       // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                                       // Circular mode: (0) disabled
	                                       // Data transfer direction: (00) Peripheral-to-memory
	                                     ; // Peripheral flow controller: (0) DMA is the flow controller

	DMA1_Stream3->FCR =                    // FIFO error interrupt: (0) disabled
	                                       // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS |  // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 1
	// set spi2 dma global interrupt priority
	NVIC_SetPriority(DMA1_Stream3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_SPI2_IRQ_PREEMPT_PRIORITY, 0));
	// enable spi2 dma global interrupt
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);
#endif
}

//--------------------------------------------
void hal_spi_i2s_init_dma_tx_buf_cycle(void)
{
	// DMA1 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	// DMA stream disable
	DMA1_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream4->CR & DMA_SxCR_EN);

	DMA1_Stream4->CR =                     // Channel selection: (000) channel 0
	                                       // Memory burst transfer configuration: (00) single transfer
	                                       // Peripheral burst transfer configuration: (00) single transfer
	                                       // Current target: (0) ignored
	                                       // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                       // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_0 | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_0 | // Peripheral data size: (01) 16-bit
	                    DMA_SxCR_MINC |    // Memory increment mode: (1) incremented after each data transfer
	                                       // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                    DMA_SxCR_CIRC |    // Circular mode: (1) enabled
	                    DMA_SxCR_DIR_0     // Data transfer direction: (01) Memory-to-peripheral
	                                     ; // Peripheral flow controller: (0) DMA is the flow controller

	DMA1_Stream4->FCR =                    // FIFO error interrupt: (0) disabled
	                                       // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS  | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 1
	// set spi2 dma global interrupt priority
	NVIC_SetPriority(DMA1_Stream4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_SPI2_IRQ_PREEMPT_PRIORITY, 0));
	// enable spi2 dma global interrupt
	NVIC_EnableIRQ(DMA1_Stream4_IRQn);
#endif
}

//--------------------------------------------
void hal_spi_i2s_init_dma_rx_buf_cycle(void)
{
	// DMA1 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	// DMA stream disable
	DMA1_Stream3->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream3->CR & DMA_SxCR_EN);

	DMA1_Stream3->CR =                     // Channel selection: (000) channel 0
	                                       // Memory burst transfer configuration: (00) single transfer
	                                       // Peripheral burst transfer configuration: (00) single transfer
	                                       // Current target: (0) ignored
	                                       // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                       // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_0 | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_0 | // Peripheral data size: (01) 16-bit
	                    DMA_SxCR_MINC |    // Memory increment mode: (1) incremented after each data transfer
	                                       // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                    DMA_SxCR_CIRC      // Circular mode: (1) enabled
	                                       // Data transfer direction: (00) Peripheral-to-memory
	                                     ; // Peripheral flow controller: (0) DMA is the flow controller

	DMA1_Stream3->FCR =                    // FIFO error interrupt: (0) disabled
	                                       // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS  | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 1
	// set spi2 dma global interrupt priority
	NVIC_SetPriority(DMA1_Stream3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_SPI2_IRQ_PREEMPT_PRIORITY, 0));
	// enable spi2 dma global interrupt
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);
#endif
}

//--------------------------------------------
void hal_spi_i2s_write_dma_buf(void *txbuf, uint32_t length)
{
	// SPI2 DMA enable
	SPI2->CR2 |= SPI_CR2_TXDMAEN;

	// DMA stream disable
	DMA1_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream4->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA1->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4 | DMA_HIFCR_CHTIF4;

	// Set the DMA addresses
	DMA1_Stream4->PAR = (uint32_t)&(SPI2->DR);
	DMA1_Stream4->M0AR = (uint32_t)txbuf;

	// Set the number of 16-bit words to transfer
	DMA1_Stream4->NDTR = length / sizeof(uint16_t);

	// Enable interrupts
	DMA1_Stream4->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA1_Stream4->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enable
	DMA1_Stream4->CR |= DMA_SxCR_EN;
	while (!(DMA1_Stream4->CR & DMA_SxCR_EN));
}

//--------------------------------------------
void hal_spi_i2s_read_dma_buf(void *rxbuf, uint32_t length)
{
	// SPI2 DMA enable
	SPI2->CR2 |= SPI_CR2_RXDMAEN;

	// DMA stream disable
	DMA1_Stream3->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream3->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA1->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3 | DMA_LIFCR_CHTIF3;

	// Set the DMA addresses
	DMA1_Stream3->PAR = (uint32_t)&(SPI2->DR);
	DMA1_Stream3->M0AR = (uint32_t)rxbuf;

	// Set the number of 16-bit words to transfer
	DMA1_Stream3->NDTR = length / sizeof(uint16_t);

	// Enable interrupts
	DMA1_Stream3->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA1_Stream3->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enable
	DMA1_Stream3->CR |= DMA_SxCR_EN;
	while (!(DMA1_Stream3->CR & DMA_SxCR_EN));
}

//--------------------------------------------
void hal_spi_i2s_write_dma_buf_stop(void *txbuf, uint32_t length, uint32_t timeout)
{
	uint32_t start;

	// SPI2 DMA enable
	SPI2->CR2 |= SPI_CR2_TXDMAEN;

	// DMA stream disable
	DMA1_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream4->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA1->HIFCR = DMA_HIFCR_CTCIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4 | DMA_HIFCR_CHTIF4;

	// Set the DMA addresses
	DMA1_Stream4->PAR = (uint32_t)&(SPI2->DR);
	DMA1_Stream4->M0AR = (uint32_t)txbuf;

	// Set the number of 16-bit words to transfer
	DMA1_Stream4->NDTR = length / sizeof(uint16_t);

	// Enable interrupts
	DMA1_Stream4->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA1_Stream4->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enable
	DMA1_Stream4->CR |= DMA_SxCR_EN;
	while (!(DMA1_Stream4->CR & DMA_SxCR_EN));

	for (start = get_platform_counter();;)
	{
		if ((get_platform_counter() - start) >= timeout)
		{
			DMA1_Stream4->CR &= ~DMA_SxCR_EN;
			while (DMA1_Stream4->CR & DMA_SxCR_EN);
		}
		if (!(DMA1_Stream4->CR & DMA_SxCR_EN))
		{
			while ((SPI2->SR & SPI_SR_BSY));
			// SPI2 DMA disable
			SPI2->CR2 &= ~SPI_CR2_TXDMAEN;
			return;
		}
	}
}

//--------------------------------------------
// Using this function while the DMA controller is running may cause
// the Channel side pointer to stop in the wrong position (SPIx_SR CHSIDE = 1),
// which will affect the next DMA start on transmitting via the I2S interface.
// Synchronization of receiving data from channel 0 does not work as expected:
// while (SPI2->SR & SPI_SR_CHSIDE);
// while (!SPI2->SR & SPI_SR_CHSIDE);
// Only one way remains - a complete reset of the settings and re-initialization:
// RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST;
// RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;
//--------------------------------------------
void hal_spi_i2s_stop_dma_tx(void)
{
	// DMA stream disable
	DMA1_Stream4->CR &= ~DMA_SxCR_EN;
	while (DMA1_Stream4->CR & DMA_SxCR_EN);
	// SPI2 DMA disable
	SPI2->CR2 &= ~SPI_CR2_TXDMAEN;
}

#if 1
//--------------------------------------------
__WEAK void hal_spi_i2s_tx_irq_htif_callback(void)
{
}

//--------------------------------------------
__WEAK void hal_spi_i2s_tx_irq_tcif_callback(void)
{
}

//--------------------------------------------
void DMA1_Stream4_IRQHandler(void)
{
	if (DMA1->HISR & DMA_HISR_TCIF4)
	{
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4;
		hal_spi_i2s_tx_irq_tcif_callback();
	}
	if (DMA1->HISR & DMA_HISR_HTIF4)
	{
		DMA1->HIFCR |= DMA_HIFCR_CHTIF4;
		hal_spi_i2s_tx_irq_htif_callback();
	}
	if (DMA1->HISR & DMA_HISR_TEIF4)
	{
		DMA1->HIFCR |= DMA_HIFCR_CTEIF4;
	}
	if (DMA1->HISR & DMA_HISR_DMEIF4)
	{
		DMA1->HIFCR |= DMA_HIFCR_CDMEIF4;
	}
	if (DMA1->HISR & DMA_HISR_FEIF4)
	{
		DMA1->HIFCR |= DMA_HIFCR_CFEIF4;
	}
}
#endif

#if 1
//--------------------------------------------
__WEAK void hal_spi_i2s_rx_irq_htif_callback(void)
{
}

//--------------------------------------------
__WEAK void hal_spi_i2s_rx_irq_tcif_callback(void)
{
}

//--------------------------------------------
void DMA1_Stream3_IRQHandler(void)
{
	if (DMA1->LISR & DMA_LISR_TCIF3)
	{
		DMA1->LIFCR |= DMA_LIFCR_CTCIF3;
		hal_spi_i2s_rx_irq_tcif_callback();
	}
	if (DMA1->LISR & DMA_LISR_HTIF3)
	{
		DMA1->LIFCR |= DMA_LIFCR_CHTIF3;
		hal_spi_i2s_rx_irq_htif_callback();
	}
	if (DMA1->LISR & DMA_LISR_TEIF3)
	{
		DMA1->LIFCR |= DMA_LIFCR_CTEIF3;
	}
	if (DMA1->LISR & DMA_LISR_DMEIF3)
	{
		DMA1->LIFCR |= DMA_LIFCR_CDMEIF3;
	}
	if (DMA1->LISR & DMA_LISR_FEIF3)
	{
		DMA1->LIFCR |= DMA_LIFCR_CFEIF3;
	}
}
#endif
