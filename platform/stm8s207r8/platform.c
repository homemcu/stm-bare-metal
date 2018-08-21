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

static volatile uint32_t counter;

//--------------------------------------------
// TIM4 interruption handler
INTERRUPT_HANDLER(TIM4_ISR_Handler, TIM4_ISR)
{
	counter++;
	TIM4_SR &= ~(1 << TIM4_SR_UIF);
}

//--------------------------------------------
void delay_ms(uint32_t delay_ms)
{
	delay_ms += counter;
	while (delay_ms != counter);
}

//--------------------------------------------
uint32_t get_platform_counter(void)
{
	return counter;
}

//--------------------------------------------
void delay_us(uint32_t delay_us)
{
	// This is incorrect us delays, but ...
	// I don't need exact delays and this function works.
#if 0
	delay_us *= 3;
#endif
	do
	{
		delay_us--;
	}
	while (delay_us);
}

//--------------------------------------------
// fHSI    = 16 MHz
// fMASTER = 16 MHz
// fCPU    = 16 MHz
static void SetSysClock(void)
{
	// fCPU = fMASTER
	// fHSI = fHSI RC
	CLK_CKDIVR = 0x00;
}

//--------------------------------------------
// fCK_PSC = fMASTER
// fCK_CNT = fCK_PSC / 2^(PSCR[2:0])
static void TIM4Init(void)
{
	// fCK_CNT = 16MHz / 2^7 = 125kHz
	TIM4_PSCR = 0x07;
	// Time base equal to 1 ms
	// ARR = 0.001s * 125000Hz - 1 = 124
	TIM4_ARR = 124;
	// enable update interrupt
	TIM4_IER |= (1 << TIM4_IER_UIE);
	// enable interrupts
	enableInterrupts();
	// enable TIM4
	TIM4_CR1 |= (1 << TIM4_CR1_CEN);
}



//--------------------------------------------
#ifdef UART_TERMINAL
#if UART_TERMINAL == 1
// PA5 --> 207-UART1 TX
// PA4 <-- 207-UART1 RX
// UART Frequency (115200)
// UART_DIV = fMASTER/UART_baud_rate = 16000000 / 115200 = 138.88 ~= 139 = 0x8B
// UART_DIV = 0x008B => BRR1 = 0x08, BRR2 = 0x0B
#define BRR1           0x08
#define BRR2           0x0B

//--------------------------------------------
static void UART1Init(void)
{
	UART1_BRR2 = BRR2;
	UART1_BRR1 = BRR1;
	UART1_CR2 = (1 << UART1_CR2_TEN) | (1 << UART1_CR2_REN);
}

//--------------------------------------------
static void putcharuart(char ch)
{
	while (!(UART1_SR & (1 << UART1_SR_TXE)));
	UART1_DR = ch;
}

//--------------------------------------------
static char getcharuart(void)
{
	char ch;

	while (!(UART1_SR & (1 << UART1_SR_RXNE)));
	ch = UART1_DR;
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
	SetSysClock();
	TIM4Init();
#ifdef UART_TERMINAL
#if UART_TERMINAL == 1
	UART1Init();
#endif
#endif
}
