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

#ifndef COMPILER_DEF_H_
#define COMPILER_DEF_H_

//-----------------------------------------------------------------------------
// SDCC - Small Device C Compiler
// http://sdcc.sourceforge.net
#if defined __SDCC

#define INTERRUPT_HANDLER(name, vector) void name (void) __interrupt (vector)
#define INTERRUPT_HANDLER_TRAP(name) void name (void) __trap

#define FLASH_MEMORY_DECLARE(type, name) type const name
#define FLASH_MEMORY_READ_BYTE(byte) *(byte)

#define enableInterrupts() __asm__("rim")
#define disableInterrupts() __asm__("sim")

#define __weak

//-----------------------------------------------------------------------------
// IAR STM8
// http://www.iar.com
#elif defined __ICCSTM8__
#include <intrinsics.h>

#define STRINGVECTOR(x) #x
#define VECTOR_ID(x) STRINGVECTOR(vector = (x))
#define INTERRUPT_HANDLER(name, vector) _Pragma(VECTOR_ID((vector)+2)) __interrupt void (name)(void)
#define INTERRUPT_HANDLER_TRAP(name) _Pragma(VECTOR_ID(1)) __interrupt void (name)(void)

#define FLASH_MEMORY_DECLARE(type, name) type const name
#define FLASH_MEMORY_READ_BYTE(byte) *(byte)

#define enableInterrupts() __enable_interrupt()
#define disableInterrupts() __disable_interrupt()

#else
#error "Unsupported compiler."

#endif

#endif // COMPILER_DEF_H_
