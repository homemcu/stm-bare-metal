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
#include "rtc-time.h"

// fRTCCLK = 32768Hz 
// fPCLK1 / fRTCCLK = 42000000 / 32768 ~= 1281 > 7
// To read the RTC calendar registers (RTC_SSR, RTC_TR and RTC_DR) properly, the APB1
// clock frequency (fPCLK1) must be equal to or greater than seven times the fRTCCLK RTC
// clock frequency. This ensures a secure behavior of the synchronization mechanism.

//--------------------------------------------
static uint8_t bin2bcd(uint8_t value)
{
	uint8_t res = 0;

	res = ((value / 10) << 4) | (value % 10);
	return res;
}

//--------------------------------------------
static uint8_t bcd2bin(uint8_t value)
{
	uint8_t res = 0;

	res = (value >> 4) * 10 + (value & 0x0F);
	return res;
}

//--------------------------------------------
static void settime(const rtc_time_t *time)
{
	uint32_t tmpreg;

	tmpreg = (uint32_t)(((uint32_t)bin2bcd(time->hour) << 16) | \
	                    ((uint32_t)bin2bcd(time->min) << 8) | \
	                    ((uint32_t)bin2bcd(time->sec)));

	RTC->TR = tmpreg;

	tmpreg = (uint32_t)(((uint32_t)bin2bcd(time->year) << 16) | \
	                    ((uint32_t)bin2bcd(time->month) << 8) | \
	                    ((uint32_t)bin2bcd(time->date)) | \
	                    ((uint32_t)time->wday << 13U));

	RTC->DR = tmpreg;
}

//--------------------------------------------
static void gettime(rtc_time_t *time)
{
	uint32_t tmpreg;

	tmpreg = RTC->TR;

	time->hour = bcd2bin((uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16U));
	time->min = bcd2bin((uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> 8U));
	time->sec = bcd2bin((uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU)));

	tmpreg = RTC->DR;

	time->year = bcd2bin((uint8_t)((tmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16U));
	time->month = bcd2bin((uint8_t)((tmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8U));
	time->date = bcd2bin((uint8_t)(tmpreg & (RTC_DR_DT | RTC_DR_DU)));
	time->wday = bcd2bin((uint8_t)((tmpreg & (RTC_DR_WDU)) >> 13U));
}

//--------------------------------------------
uint8_t hal_int_rtc_init(void)
{
	// Power interface clock enable
	// ** __HAL_RCC_PWR_CLK_ENABLE
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

#if 0
	// ** Battery replacement simulation **
	// Write access to RTC and RTC Backup registers and backup SRAM enabled
	PWR->CR |= PWR_CR_DBP;
   	while (!(PWR->CR & PWR_CR_DBP));
	// Backup domain software reset
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	// Write access to RTC and RTC Backup registers and backup SRAM disabled
	PWR->CR &= ~PWR_CR_DBP;
   	while ((PWR->CR & PWR_CR_DBP));
#endif

	// Check if LSE oscillator clock is used as the RTC clock.
	// Once the RTC clock source has been selected, it cannot be
	// changed anymore unless the Backup domain is reset.
	if ((RCC->BDCR & RCC_BDCR_RTCSEL) != RCC_BDCR_RTCSEL_0)
	{
		// ** HAL_RCC_OscConfig
		// ** HAL_RCCEx_PeriphCLKConfig
		// ** __HAL_RCC_RTC_ENABLE
		// Write access to RTC and RTC Backup registers and backup SRAM enabled
		PWR->CR |= PWR_CR_DBP;
		while (!(PWR->CR & PWR_CR_DBP));
		// Backup domain software reset
		RCC->BDCR |= RCC_BDCR_BDRST;
		RCC->BDCR &= ~RCC_BDCR_BDRST;
		// LSE clock selected and ON
		RCC->BDCR |= RCC_BDCR_LSEON | RCC_BDCR_RTCSEL_0 | RCC_BDCR_RTCEN;
		while (!(RCC->BDCR & RCC_BDCR_LSERDY));
		// RTC clock enable
		RCC->BDCR |= RCC_BDCR_RTCEN;
		// Write access to RTC and RTC Backup registers and backup SRAM disabled
		PWR->CR &= ~PWR_CR_DBP;
		while ((PWR->CR & PWR_CR_DBP));
	}

#if 0
	// Check if RTC is right installed
	if (RTC->BKP1R != 0xAAAAAAAA)
#else
	// After a system reset, the application can read the INITS flag in the RTC_ISR register to
	// check if the calendar has been initialized or not. If this flag equals 0, the calendar has not
	// been initialized since the year field is set at its backup domain reset default value (0x00).
	if (!(RTC->ISR & RTC_ISR_INITS))
#endif
	{
		rtc_time_t time = {18, 1, 1, 0, 0, 0, 1 };

		// ** HAL_RTC_Init
		// ** HAL_RTC_SetTime
		// ** HAL_RTC_SetDate
		// Write access to RTC and RTC Backup registers and backup SRAM enabled
		PWR->CR |= PWR_CR_DBP;
		while (!(PWR->CR & PWR_CR_DBP));
		// Disable the RTC registers write protection
		RTC->WPR = 0xCA;
		RTC->WPR = 0x53;

		// Enter Initialization mode
		RTC->ISR |= RTC_ISR_INIT;
		while (!(RTC->ISR & RTC_ISR_INITF));

		// Set date and time
		settime(&time);

		if (!(RTC->CR & RTC_CR_BYPSHAD))
		{
			// Clear RSF flag: Calendar shadow registers not yet synchronized
			RTC->ISR &= ~RTC_ISR_RSF;
	  		while ((RTC->ISR & RTC_ISR_RSF));
		}

#if 0
		// RTC is right installed
		RTC->BKP1R = 0xAAAAAAAA;
#endif

		// Exit Initialization mode
		RTC->ISR &= ~RTC_ISR_INIT;

		// Enable the RTC registers write protection
		RTC->WPR = 0xFF;
		// Write access to RTC and RTC Backup registers and backup SRAM disabled
		PWR->CR &= ~PWR_CR_DBP;
		while ((PWR->CR & PWR_CR_DBP));
		return RTC_TIME_NOT_VALID;
	}
	return RTC_TIME_VALID;
}

//--------------------------------------------
void hal_int_rtc_settime(const rtc_time_t *time)
{
	// Write access to RTC and RTC Backup registers and backup SRAM enabled
	PWR->CR |= PWR_CR_DBP;
	while (!(PWR->CR & PWR_CR_DBP));
	// Disable the RTC registers write protection
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// Enter Initialization mode
	RTC->ISR |= RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF));

	// Set date and time
	settime(time);

	if (!(RTC->CR & RTC_CR_BYPSHAD))
	{
		// Clear RSF flag: Calendar shadow registers not yet synchronized
		RTC->ISR &= ~RTC_ISR_RSF;
  		while ((RTC->ISR & RTC_ISR_RSF));
	}

	// Exit Initialization mode
	RTC->ISR &= ~RTC_ISR_INIT;

	// Enable the RTC registers write protection
	RTC->WPR = 0xFF;
	// Write access to RTC and RTC Backup registers and backup SRAM disabled
	PWR->CR &= ~PWR_CR_DBP;
	while ((PWR->CR & PWR_CR_DBP));
}

//--------------------------------------------
void hal_int_rtc_gettime(rtc_time_t *time)
{
	gettime(time);
}
