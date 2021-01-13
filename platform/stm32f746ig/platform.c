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
#include "stm32f7xx-hw.h"

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
// fHCLK =  216 MHz (AHB)
// fPCLK1 =  54 MHz (APB1)
// fPCLK2 = 108 MHz (APB2)
#define PLL_M      6
#define PLL_N      324
#define PLL_P      2
#define PLL_Q      9
static void SetSysClock(void)
{
	// HSE clock enable
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Power interface clock enable
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	// Scale 1 mode (default value at reset):
	// the device does operate at the maximum frequency 216 MHz
	PWR->CR1 |= PWR_CR1_VOS;

	// AHB = SYSCLK / 1
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	// APB2 = AHB / 2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

	// APB1 = AHB / 4
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

	// configure the main PLL
	RCC->PLLCFGR = (PLL_M << RCC_PLLCFGR_PLLM_Pos) |
	               (PLL_N << RCC_PLLCFGR_PLLN_Pos) |
                   (((PLL_P >> 1) - 1) << RCC_PLLCFGR_PLLP_Pos) |
	               (PLL_Q << RCC_PLLCFGR_PLLQ_Pos) |
	               RCC_PLLCFGR_PLLSRC_HSE;

	// enable the main PLL
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// configure Flash prefetch and wait state
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;

	// select the main PLL as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

//--------------------------------------------
static void DWTInit(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->LAR = 0xC5ACCE55;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; 
}



//--------------------------------------------
#ifdef UART_TERMINAL
//--------------------------------------------
// USART1(APB2, SYSCLK, HSI, LSE)
// GPIO_AF7_USART1
// TX:   PA9    PB6
// RX:   PA10   PB7
//
// USART2(APB1, SYSCLK, HSI, LSE)
// GPIO_AF7_USART2
// TX:   PA2    PD5
// RX:   PA3    PD6
//
// USART3(APB1, SYSCLK, HSI, LSE)
// GPIO_AF7_USART3
// TX:   PB10   PC10   PD8
// RX:   PB11   PC11   PD9
//
// UART4(APB1, SYSCLK, HSI, LSE)
// GPIO_AF8_UART4
// TX:   PA0    PC10
// RX:   PA1    PC11
//
// UART5(APB1, SYSCLK, HSI, LSE)
// GPIO_AF8_UART5
// TX:   PC12
// RX:   PD2
//
// USART6(APB2, SYSCLK, HSI, LSE)
// GPIO_AF8_USART6
// TX:   PC6    PG14
// RX:   PC7    PG9
//
// UART7(APB1, SYSCLK, HSI, LSE)
// GPIO_AF8_UART7
// TX:   PE8    PF7
// RX:   PE7    PF6
//
// UART8(APB1, SYSCLK, HSI, LSE)
// GPIO_AF8_UART8
// TX:   PE1
// RX:   PE0
//
// The first clock source in the brackets
// is selected as U(S)ART clock by default,
// see RCC->DCKCFGR2 register

//--------------------------------------------
#if UART_TERMINAL == 1
#define	PORT_TX        GPIO_B   // PB6  --> RXD
#define	PIN_TX         6
#define	PORT_RX        GPIO_B   // PB7  <-- TXD
#define	PIN_RX         7

//--------------------------------------------
// USART Baud rate
// USARTDIV = PCLK2(108MHz) * N / Baud
// N = 1 if OVER8 = 0 (Oversampling by 16)
// N = 2 if OVER8 = 1 (Oversampling by 8)
// if OVER8 = 0 (Oversampling by 16), Baud = 115200
// USARTDIV = PCLK2(108MHz) * 1 / 115200 = 937.5 ~= 0x3A9
#define USART1_DIV       0x3A9

//--------------------------------------------
static void UART1Init(void)
{
	// USART1 clock enable
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// IO port B clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	hw_cfg_pin(GPIOx(PORT_TX), PIN_TX, GPIOCFG_MODE_ALT | GPIO_AF7_USART1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_RX), PIN_RX, GPIOCFG_MODE_ALT | GPIO_AF7_USART1 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	USART1->BRR = USART1_DIV;
	// OVER8 = 0: oversampling by 16
	// TE = 1: Transmitter is enabled
	// RE = 1: Receiver is enabled and begins searching for a start bit
	// UE = 1: USART enabled
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

//--------------------------------------------
void putcharuart(char ch)
{
	while (!(USART1->ISR & USART_ISR_TXE));
	USART1->TDR = ch;
}

//--------------------------------------------
char getcharuart(void)
{
	char ch;

	while (!(USART1->ISR & USART_ISR_RXNE));
	ch = USART1->RDR;
	return ch;
}

//--------------------------------------------
#elif UART_TERMINAL == 7
#define	PORT_TX        GPIO_F   // PF7  --> RXD
#define	PIN_TX         7
#define	PORT_RX        GPIO_F   // PF6  <-- TXD
#define	PIN_RX         6

//--------------------------------------------
// USART Baud rate
// USARTDIV = PCLK1(54MHz) * N / Baud
// N = 1 if OVER8 = 0 (Oversampling by 16)
// N = 2 if OVER8 = 1 (Oversampling by 8)
// if OVER8 = 0 (Oversampling by 16), Baud = 115200
// USARTDIV = PCLK1(54MHz) * 1 / 115200 = 468.75 ~= 0x1D5
#define UART7_DIV       0x1D5

//--------------------------------------------
static void UART7Init(void)
{
	// UART7 clock enable
	RCC->APB1ENR |= RCC_APB1ENR_UART7EN;
	// IO port F clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;

	hw_cfg_pin(GPIOx(PORT_TX), PIN_TX, GPIOCFG_MODE_ALT | GPIO_AF8_UART7 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	hw_cfg_pin(GPIOx(PORT_RX), PIN_RX, GPIOCFG_MODE_ALT | GPIO_AF8_UART7 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	UART7->BRR = UART7_DIV;
	// OVER8 = 0: oversampling by 16
	// TE = 1: Transmitter is enabled
	// RE = 1: Receiver is enabled and begins searching for a start bit
	// UE = 1: USART enabled
	UART7->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

//--------------------------------------------
void putcharuart(char ch)
{
	while (!(UART7->ISR & USART_ISR_TXE));
	UART7->TDR = ch;
}

//--------------------------------------------
char getcharuart(void)
{
	char ch;

	while (!(UART7->ISR & USART_ISR_RXNE));
	ch = UART7->RDR;
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
#if 1
	// Number of group priorities: 16
	// Number of sub priorities: none
	// All the priority bits are the preempt priority bits only
	NVIC_SetPriorityGrouping(3);
#endif
	SetSysClock();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	// Changing the SysTick_IRQn priority level in the new group
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	DWTInit();
#ifdef UART_TERMINAL
#if UART_TERMINAL == 1
	UART1Init();
#elif UART_TERMINAL == 7
	UART7Init();
#endif
#endif
}
