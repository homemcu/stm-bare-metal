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

#ifndef SDCC_INTERRUPTS_H_
#define SDCC_INTERRUPTS_H_

//--------------------------------------------
// SDCC Compiler User Guide.
// 3.10.1 General Information
// If you have multiple source files in your project,
// interrupt service routines can be present in any of them,
// but a prototype of the isr MUST be present or included
// in the file that contains the function main.

INTERRUPT_HANDLER(TIM4_ISR_Handler, TIM4_ISR);

#endif // SDCC_INTERRUPTS_H_
