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
// FRAMERATE FEEDBACK:
// TIM2 is used for measure real DAC framerate. I2S_MCK pin must be connected to TIM2_ETR.
// ETR pin is used as clock source for timer. OTG_FS/OTG_HS core generates SOF pulse which is internally routed to ITR1 of TIM2.
//--------------------------------------------

//--------------------------------------------
// TIM2(APB1, )
// GPIO_AF1_TIM2
// CH1:   PA0    PA5    PA15
// CH2:   PA1    PB3
// CH3:   PA2    PB10
// CH4:   PA3    PB11
// ETR:   PA0    PA5    PA15

//--------------------------------------------
#define	PORT_MCLK      GPIO_A  // PA0  <-- MCLK
#define	PIN_MCLK       0

//--------------------------------------------
#ifdef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
// = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY in FreeRTOSConfig.h
#define TIM2_IRQ_PREEMPT_PRIORITY     (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#else
#define TIM2_IRQ_PREEMPT_PRIORITY     6
#endif

//--------------------------------------------
static uint32_t mclk;
static uint32_t mclk_count;
static uint32_t sof_max;
static uint32_t sof_count;

//--------------------------------------------
void hal_i2s_mclk_init(void)
{
	// TIM2 clock enable
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	hw_cfg_pin(GPIOx(PORT_MCLK), PIN_MCLK, GPIOCFG_MODE_ALT | GPIO_AF1_TIM2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);

	NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), TIM2_IRQ_PREEMPT_PRIORITY, 0));
}

//--------------------------------------------
void hal_i2s_mclk_set_sof(uint32_t sof)
{
	sof_max = sof;
}

//--------------------------------------------
void hal_i2s_mclk_start_count(void)
{
	mclk_count = 0;
	mclk = 0;
	sof_count = 0;

	// TIM2 count reset
	TIM2->CNT = 0;
	// Timer clock = ETR pin, slave mode, trigger on ITR1
	TIM2->SMCR = TIM_SMCR_ECE | TIM_SMCR_TS_0 | TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;
#ifdef USBD_OTGFS
	// Remap ITR1 to USB_FS SOF signal
	TIM2->OR = TIM_OR_ITR1_RMP_1;
#else
	// Remap ITR1 to USB_HS SOF signal
	TIM2->OR = TIM_OR_ITR1_RMP_0 | TIM_OR_ITR1_RMP_1;
#endif
	// TIM2 enable interrupt
	TIM2->DIER = TIM_DIER_TIE;
	// Enable TIM2 counting
	TIM2->CR1 = TIM_CR1_CEN;

	NVIC_EnableIRQ(TIM2_IRQn);
}

//--------------------------------------------
void hal_i2s_mclk_stop_count(void)
{
	NVIC_DisableIRQ(TIM2_IRQn);

	// Disable slave move, can't stop the timer unless we do this first
	TIM2->SMCR = 0;
	// Stop TIM2
	TIM2->CR1 = 0;
}

//--------------------------------------------
__WEAK void mclk_irq_callback(uint32_t mclk)
{
}

//--------------------------------------------
void TIM2_IRQHandler(void)
{
	uint32_t count;
	uint32_t sr;

	count = TIM2->CNT;
	sr = TIM2->SR;
	TIM2->SR = 0;

	if (sr & TIM_SR_TIF)
	{
		if (count)
		{
			if (mclk_count < count)
			{
				mclk += count - mclk_count;
			}
			else
			{
				mclk += UINT32_MAX - mclk_count + count;
			}
			if (sof_count == sof_max)
			{
				mclk_irq_callback(mclk);
				mclk = 0;
				sof_count = 0;
			}
			else
			{
				sof_count++;
			}
			mclk_count = count;
		}
	}
}
