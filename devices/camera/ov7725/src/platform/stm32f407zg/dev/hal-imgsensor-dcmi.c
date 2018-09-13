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
// DCMI(AHB2)
// GPIO_AF13_DCMI
// DCMI D0:      PA9    PC6    PH9
// DCMI D1:      PA10   PC7    PH10
// DCMI D2:      PE0    PC8    PH11
// DCMI D3:      PE1    PC9    PH12
// DCMI D4:      PE4    PC11   PH14
// DCMI D5:      PB6    PI4
// DCMI D6:      PB8    PE5    PI6
// DCMI D7:      PB9    PE6    PI7
// DCMI D8:      PI1    PC10
// DCMI D9:      PI2    PC12
// DCMI D10:     PB5    PI3
// DCMI D11:     PD2    PH15
// DCMI D12:     PF11
// DCMI D13:     PG15   PI0
// DCMI HSYNC:   PA4    PH8
// DCMI VSYNC:   PB7    PI5
// DCMI PIXCK:   PA6

//--------------------------------------------
// WSYNC, HSYNC and PCLK wires must be the same length as data wires
#define	PORT_D0         GPIO_C // PC6 <-- D0
#define	PIN_D0          6
#define	PORT_D1         GPIO_C // PC7 <-- D1
#define	PIN_D1          7
#define	PORT_D2         GPIO_E // PE0 <-- D2
#define	PIN_D2          0
#define	PORT_D3         GPIO_E // PE1 <-- D3
#define	PIN_D3          1
#define	PORT_D4         GPIO_E // PE4 <-- D4
#define	PIN_D4          4
#define	PORT_D5         GPIO_B // PB6 <-- D5
#define	PIN_D5          6
#define	PORT_D6         GPIO_E // PE5 <-- D6
#define	PIN_D6          5
#define	PORT_D7         GPIO_E // PE6 <-- D7
#define	PIN_D7          6
#define	PORT_HSYNC      GPIO_A // PA4 <-- HS
#define	PIN_HSYNC       4
#define	PORT_VSYNC      GPIO_B // PB7 <-- VS
#define	PIN_VSYNC       7
#define	PORT_PIXCK      GPIO_A // PA6 <-- PCLK
#define	PIN_PIXCK       6

//--------------------------------------------
// TIM1(APB2)
// GPIO_AF1_TIM1
// TIM1_CH1:    PA8    PE9
// TIM1_CH1N:   PA7    PB13   PE8

//--------------------------------------------
// XCLK wire must be as short as possible
#define	PORT_TIM1_CH1   GPIO_A // PA8 --> XCLK(MCLK)
#define	PIN_TIM1_CH1    8


#if 0
#define DCMI_IRQ_PREEMPT_PRIORITY        1
#define DMA_DCMI_IRQ_PREEMPT_PRIORITY    0
#endif

//--------------------------------------------
void hal_imgsensor_init_capture(void)
{
	// IO port A clock enable
	// IO port B clock enable
	// IO port C clock enable
	// IO port E clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOEEN;
	// DCMI clock enable
	RCC->AHB2ENR |= RCC_AHB2ENR_DCMIEN;

	hw_cfg_pin(GPIOx(PORT_D0),    PIN_D0,    GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D1),    PIN_D1,    GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D2),    PIN_D2,    GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D3),    PIN_D3,    GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D4),    PIN_D4,    GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D5),    PIN_D5,    GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D6),    PIN_D6,    GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D7),    PIN_D7,    GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_HSYNC), PIN_HSYNC, GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_VSYNC), PIN_VSYNC, GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_PIXCK), PIN_PIXCK, GPIOCFG_MODE_ALT | GPIO_AF13_DCMI | GPIOCFG_OSPEED_LOW | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);

	// EDM[1:0] = 00: Interface captures 8-bit data on every pixel clock
	// FCRC[1:0] = 00: All frames are captured
	// VSPOL = 1: VSYNC active high:
	//            when the VSYNC is low, the data is valid
	//            when the VSYNC is at the high level, the data is not valid (vertical blanking)
	// HSPOL = 0: HSYNC active low:
	//            when the HSYNC is high, the data is valid
	//            when the HSYNC is at the low level, the data is not valid (horizontal blanking)
	// PCKPOL = 1: Rising edge active
	// ESS = 0: Hardware synchronization data capture (frame/line start/stop)
	//          is synchronized with the HSYNC/VSYNC signals.
	// JPEG = 0: Uncompressed video format
	// CROP = 0: The full image is captured. In this case the total number of bytes
	//           in an image frame should be a multiple of 4
	// CM = 0: Continuous grab mode - The received data are transferred into the destination
	//         memory through the DMA. The buffer location and mode (linear or circular
	//         buffer) is controlled through the system DMA.
	DCMI->CR = DCMI_CR_VSPOL | DCMI_CR_PCKPOL;

#if 0
	// Enable all interrupts
	DCMI->IER = DCMI_IER_LINE_IE | DCMI_IER_VSYNC_IE | DCMI_IER_ERR_IE | DCMI_IER_OVR_IE | DCMI_IER_FRAME_IE;
	// set dcmi dma global interrupt priority
	NVIC_SetPriority(DCMI_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DCMI_IRQ_PREEMPT_PRIORITY, 0));
	// enable dcmi dma global interrupt
	NVIC_EnableIRQ(DCMI_IRQn);
#endif
}

//--------------------------------------------
// TIM1 
// fPCLK2 -> (int PLL) -> fCK_PSC -> (TIMx_PSC) -> fCK_CNT -> (TIMx_ARR) -> fCK_OUT
void hal_imgsensor_init_clock(void)
{
	// TIM1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	// IO port A clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	hw_cfg_pin(GPIOx(PORT_TIM1_CH1), PIN_TIM1_CH1, GPIOCFG_MODE_ALT | GPIO_AF1_TIM1 | GPIOCFG_OSPEED_MEDIUM | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);

	// fCK_PSC = fPCLK2 * 2
	// fCK_CNT = fCK_PSC / (TIMx_PSC + 1)
	// fCK_CNT = 84 * 2 / (0 + 1) = 168MHz
	TIM1->PSC = 0;

	// period = (fCK_CNT / fCK_OUT)
	// period = 168 / 24 = 7
	// ARR = period - 1
	// ARR = 7 - 1 = 6
	TIM1->ARR = 6;

	// Set the UG bit to enable UEV
	TIM1->EGR |= TIM_EGR_UG;

	// After reset, all the timer channels are turned off
#if 0
	TIM1->CCER &= ~TIM_CCER_CC1E;
#endif

	// OC1M[6:4] = 110: PWM mode 1 - In upcounting, channel 1 is active
	//                  as long as TIMx_CNT<TIMx_CCR1 else inactive.
	// CC1S[1:0] = 00: CC1 channel is configured as output
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;

	// Set the duty cycle to 50%
	// CCR1 = period / 2
	// CCR1 = 7 / 2 = 3
	TIM1->CCR1 = 3;

	// Enable the TIM1 channel1 and keep the default configuration
	// (state after reset) for channel polarity
	TIM1->CCER |= TIM_CCER_CC1E;

	// Enable the TIM1 Main Output
	TIM1->BDTR |= TIM_BDTR_MOE;
	// Enable the TIM Counter
	TIM1->CR1 |= TIM_CR1_CEN;
}

//--------------------------------------------
void hal_imgsensor_enable_clock(void)
{
	TIM1->CR1 |= TIM_CR1_CEN;
}

//--------------------------------------------
void hal_imgsensor_disable_clock(void)
{
	TIM1->CR1 &= ~TIM_CR1_CEN;
}

//--------------------------------------------
void hal_imgsensor_init_dma_cycle(void)
{
	// DMA2 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// DMA stream disabled
	DMA2_Stream1->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream1->CR & DMA_SxCR_EN);

	DMA2_Stream1->CR =  DMA_SxCR_CHSEL_0  | // Channel selection: (001) channel 1
	                                        // Memory burst transfer configuration: (00) single transfer
	                                        // Peripheral burst transfer configuration: (00) single transfer
	                                        // Current target: (0) ignored
	                                        // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                        // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_0  | // Memory data size: (01) 16-bit
	                    DMA_SxCR_PSIZE_1  | // Peripheral data size: (10) 32-bit
	                                        // Memory increment mode: (0) Memory address pointer is fixed
	                                        // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                    DMA_SxCR_CIRC |     // Circular mode: (1) enabled
	                                        // Data transfer direction: (00) Peripheral-to-memory
	                    DMA_SxCR_PFCTRL;    // Peripheral flow controller: (1) The peripheral is the flow controller

	DMA2_Stream1->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 0
	// set dcmi dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_DCMI_IRQ_PREEMPT_PRIORITY, 0));
	// enable dcmi dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream1_IRQn);
#endif
}

//--------------------------------------------
void hal_imgsensor_init_dma_buf(void)
{
	// DMA2 clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// DMA stream disabled
	DMA2_Stream1->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream1->CR & DMA_SxCR_EN);

	DMA2_Stream1->CR =  DMA_SxCR_CHSEL_0  | // Channel selection: (001) channel 1
	                                        // Memory burst transfer configuration: (00) single transfer
	                                        // Peripheral burst transfer configuration: (00) single transfer
	                                        // Current target: (0) ignored
	                                        // Double buffer mode: (0) No buffer switching at the end of transfer
	                    DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // Priority level: (11) Very high
	                                        // Peripheral increment offset size: (0) ignored
	                    DMA_SxCR_MSIZE_1  | // Memory data size: (10) 32-bit
	                    DMA_SxCR_PSIZE_1  | // Peripheral data size: (10) 32-bit
	                    DMA_SxCR_MINC       // Memory address pointer is incremented after each data transfer
	                                        // Peripheral increment mode: (0) Peripheral address pointer is fixed
	                                        // Circular mode: (0) disabled
	                                        // Data transfer direction: (00) Peripheral-to-memory
	                                      ; // Peripheral flow controller: (0) The DMA is the flow controller

	DMA2_Stream1->FCR =                     // FIFO error interrupt: (0) disabled
	                                        // FIFO status: These bits are read-only
	                    DMA_SxFCR_DMDIS   | // Direct mode: (1) disable
	                    DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1; // FIFO threshold selection: (11) full FIFO

#if 0
	// set dcmi dma global interrupt priority
	NVIC_SetPriority(DMA2_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_DCMI_IRQ_PREEMPT_PRIORITY, 0));
	// enable dcmi dma global interrupt
	NVIC_EnableIRQ(DMA2_Stream1_IRQn);
#endif
}

//--------------------------------------------
void hal_imgsensor_start_dma_cycle(uint8_t *buf)
{
	// DCMI enabled
	// The DCMI configuration registers should be 
	// programmed correctly before enabling this bit.
	DCMI->CR |= DCMI_CR_ENABLE;

	// DMA stream disabled
	DMA2_Stream1->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream1->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA2->LIFCR = DMA_LIFCR_CTCIF1 | DMA_LIFCR_CTEIF1 | DMA_LIFCR_CDMEIF1 | DMA_LIFCR_CFEIF1 | DMA_LIFCR_CHTIF1;

	// Set the DMA addresses
	DMA2_Stream1->PAR = (uint32_t)&(DCMI->DR);
	DMA2_Stream1->M0AR = (uint32_t)&buf[0];

	// Enable interrupts
	DMA2_Stream1->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA2_Stream1->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enabled
	DMA2_Stream1->CR |= DMA_SxCR_EN;
	while (!(DMA2_Stream1->CR & DMA_SxCR_EN));

	// CAPTURE = 1: Capture enabled
	//              The DMA controller and all DCMI configuration registers
	//              should be programmed correctly before enabling this bit.
	DCMI->CR |= DCMI_CR_CAPTURE;
}

//--------------------------------------------
uint32_t hal_imgsensor_read_dma_buf(uint8_t *buf, uint32_t length, uint32_t timeout)
{
	uint32_t start;

	// Capture mode
	DCMI->CR |= DCMI_CR_CM;

	// DCMI enabled
	// The DCMI configuration registers should be 
	// programmed correctly before enabling this bit.
	DCMI->CR |= DCMI_CR_ENABLE;

	// DMA stream disabled
	DMA2_Stream1->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream1->CR & DMA_SxCR_EN);

	// Clear all the interrupt flags
	DMA2->LIFCR = DMA_LIFCR_CTCIF1 | DMA_LIFCR_CTEIF1 | DMA_LIFCR_CDMEIF1 | DMA_LIFCR_CFEIF1 | DMA_LIFCR_CHTIF1;

	// Set the DMA addresses
	DMA2_Stream1->PAR = (uint32_t)&(DCMI->DR);
	DMA2_Stream1->M0AR = (uint32_t)&buf[0];

	// Set the number of 32-bit words to transfer
	DMA2_Stream1->NDTR = length / sizeof(uint32_t);

	// Enable interrupts
	DMA2_Stream1->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_HTIE;
	DMA2_Stream1->FCR |= DMA_SxFCR_FEIE;

	// DMA stream enabled
	DMA2_Stream1->CR |= DMA_SxCR_EN;
	while (!(DMA2_Stream1->CR & DMA_SxCR_EN));

	// CAPTURE = 1: Capture enabled
	//              The DMA controller and all DCMI configuration registers
	//              should be programmed correctly before enabling this bit.
	DCMI->CR |= DCMI_CR_CAPTURE;

	for (start = get_platform_counter();;)
	{
		if ((get_platform_counter() - start) >= timeout)
			return DMA2_Stream1->NDTR;
		if (!(DMA2_Stream1->CR & DMA_SxCR_EN))
			return 0;
	}
}

//--------------------------------------------
void hal_imgsensor_stop_dma(void)
{
	// Capture disabled
	DCMI->CR &= ~DCMI_CR_CAPTURE;
	while (DCMI->CR & DCMI_CR_CAPTURE);
	// DMA stream disabled
	DMA2_Stream1->CR &= ~DMA_SxCR_EN;
	while (DMA2_Stream1->CR & DMA_SxCR_EN);
	// DCMI disabled
	DCMI->CR &= ~DCMI_CR_ENABLE;
}

//--------------------------------------------
void hal_imgsensor_wait_vsync(void)
{
	while (!(DCMI->SR & DCMI_SR_VSYNC));
}

//--------------------------------------------
void hal_imgsensor_wait_hsync(void)
{
	while (!(DCMI->SR & DCMI_SR_HSYNC));
}

//--------------------------------------------
uint32_t hal_imgsensor_read_fifo(void)
{
	uint32_t data;
	while (!(DCMI->SR & DCMI_SR_FNE));
	data = DCMI->DR;
	return data;
}

//--------------------------------------------
void hal_imgsensor_start_capture(void)
{
	// DCMI enabled
	DCMI->CR |= DCMI_CR_ENABLE;
	// Capture enabled
	DCMI->CR |= DCMI_CR_CAPTURE;
}

//--------------------------------------------
void hal_imgsensor_stop_capture(void)
{
	// Capture disabled
	DCMI->CR &= ~DCMI_CR_CAPTURE;
	while (DCMI->CR & DCMI_CR_CAPTURE);
	// DCMI disabled
	DCMI->CR &= ~DCMI_CR_ENABLE;
}

#if 0
//--------------------------------------------
void DMA2_Stream1_IRQHandler(void)
{
	if (DMA2->LISR & DMA_LISR_TCIF1)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTCIF1;
	}
	if (DMA2->LISR & DMA_LISR_HTIF1)
	{
		DMA2->LIFCR |= DMA_LIFCR_CHTIF1;
	}
	if (DMA2->LISR & DMA_LISR_TEIF1)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTEIF1;
	}
	if (DMA2->LISR & DMA_LISR_DMEIF1)
	{
		DMA2->LIFCR |= DMA_LIFCR_CDMEIF1;
	}
	if (DMA2->LISR & DMA_LISR_FEIF1)
	{
		DMA2->LIFCR |= DMA_LIFCR_CFEIF1;
	}
}
#endif
#if 0
//--------------------------------------------
void DCMI_IRQHandler(void)
{
	if (DCMI->MISR & DCMI_MISR_LINE_MIS)
	{
		DCMI->ICR |= DCMI_ICR_LINE_ISC;
	}
	if (DCMI->MISR & DCMI_MISR_VSYNC_MIS)
	{
		DCMI->ICR |= DCMI_ICR_VSYNC_ISC;
	}
	if (DCMI->MISR & DCMI_MISR_ERR_MIS)
	{
		DCMI->ICR |= DCMI_ICR_ERR_ISC;
	}
	if (DCMI->MISR & DCMI_MISR_OVR_MIS)
	{
		DCMI->ICR |= DCMI_ICR_OVR_ISC;
	}
	if (DCMI->MISR & DCMI_MISR_FRAME_MIS)
	{
		DCMI->ICR |= DCMI_ICR_FRAME_ISC;
	}
}
#endif