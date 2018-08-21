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

#include "cpu.h"
#include "stm32f4xx-hw.h"

static volatile uint32_t counter = 0;

//--------------------------------------------
// SysTick timer interruption handler
void SysTick_Handler(void)
{
	counter++;
}

//--------------------------------------------
void delay_ms(uint32_t delay_ms)
{
	uint32_t start;
	start = counter;
	while ((counter - start) < delay_ms);
}

//--------------------------------------------
uint32_t get_platform_counter(void)
{
	return counter;
}

//--------------------------------------------
void delay_us(uint32_t delay_us)
{
	uint32_t start;
	start = DWT->CYCCNT;
	delay_us *= (SystemCoreClock / 1000000);
	while ((DWT->CYCCNT - start) < delay_us);
}

//--------------------------------------------
// HSE crystal oscillator 8 MHz
// Define HSE_VALUE=8000000 in Makefile(GCC ARM) or Preprocessor tab(IAR)
// fHCLK = 168 MHz (AHB)
// fPCLK1 = 42 MHz (APB1)
// fPCLK2 = 84 MHz (APB2)
#define PLL_M      8
#define PLL_N      336
#define PLL_P      2
#define PLL_Q      7
static void SetSysClock(void)
{
	// HSE clock enable
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Power interface clock enable
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	// Scale 1 mode (default value at reset):
	// the device does operate at the maximum frequency 168 MHz
	PWR->CR |= PWR_CR_VOS;

	// AHB = SYSCLK / 1
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	// APB2 = AHB / 2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

	// APB1 = AHB / 4
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

	// configure the main PLL
	RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | RCC_PLLCFGR_PLLSRC_HSE | (PLL_Q << 24);

	// enable the main PLL
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// configure Flash prefetch, Instruction cache, Data cache and wait state
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

	// select the main PLL as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

//--------------------------------------------
static void DWTInit(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; 
}



//--------------------------------------------
#ifdef UART_TERMINAL
#if UART_TERMINAL == 1
#define	PORT_TX        GPIO_A   // PA9 --> RXD
#define	PIN_TX         9
#define	PORT_RX        GPIO_A   // PA10 <-- TXD
#define	PIN_RX         10
//--------------------------------------------
// USART Baud rate
// USARTDIV = PCLK2(84MHz) / 8 x (2 - OVER8) x Baud
// if OVER8 = 0 (4-bit USARTDIV_Fraction), Baud = 115200
// USARTDIV = PCLK2(84MHz) / 8 x 2 x 115200 ~= 45.573
// USARTDIV_Mantissa = 45
#define USART1_DIV_M       45
// USARTDIV_Fraction = 0.573 / (1 / 16) = 0.573 / 0.0625 ~= 9
#define USART1_DIV_F       9

//--------------------------------------------
static void UART1Init(void)
{
	// USART1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// IO port A clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	hw_cfg_pin(GPIOx(PORT_TX), PIN_TX, GPIOCFG_MODE_ALT | GPIO_AF7_USART1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_RX), PIN_RX, GPIOCFG_MODE_ALT | GPIO_AF7_USART1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	USART1->BRR = (USART1_DIV_M << 4) | USART1_DIV_F;
	// OVER8 = 0: oversampling by 16
	// UE = 1: USART enable
	USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

//--------------------------------------------
void putcharuart(char ch)
{
	USART1->DR = ch;
	while (!(USART1->SR & USART_SR_TXE));
}

//--------------------------------------------
char getcharuart(void)
{
	char ch;

	while (!(USART1->SR & USART_SR_RXNE));
	ch = USART1->DR;
	return ch;
}
#endif

//--------------------------------------------
int putchar(int data)
{
	putcharuart((char)data);
	return data;
}

//--------------------------------------------
int getchar(void)
{
	return (int)getcharuart();
}
#endif



//--------------------------------------------
void platform_init(void)
{
	SystemInit();
	SetSysClock();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	DWTInit();
#ifdef UART_TERMINAL
#if UART_TERMINAL == 1
	UART1Init();
#endif
#endif
}
