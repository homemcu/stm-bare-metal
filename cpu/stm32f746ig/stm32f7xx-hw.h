/*
 * Copyright (c) 2014-2016 IBM Corporation.
 * All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
* This file has been modified for STM32F7xx
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

#ifndef STM32F7XX_HW_H_
#define STM32F7XX_HW_H_

// GPIO by port number (A=0, B=1, ..)
#define GPIOx(no)               ((GPIO_TypeDef*)(GPIOA_BASE + (no) * (GPIOB_BASE-GPIOA_BASE)))
#define GPIO_A                  0
#define GPIO_B                  1
#define GPIO_C                  2
#define GPIO_D                  3
#define GPIO_E                  4
#define GPIO_F                  5
#define GPIO_G                  6
#define GPIO_H                  7
#define GPIO_I                  8
#define GPIO_J                  9
#define GPIO_K                  10

// GPIOCFG macros
#define GPIO_AF_BITS            4       // width of bit field
#define GPIO_AF_MASK            0x000F  // mask in AFR[0/1]
#define GPIO_AF0_MCO            0
#define GPIO_AF1_TIM1           1
#define GPIO_AF1_TIM2           1
#define GPIO_AF4_I2C1           4
#define GPIO_AF4_I2C2           4
#define GPIO_AF4_I2C3           4
#define GPIO_AF4_I2C4           4
#define GPIO_AF5_SPI1           5
#define GPIO_AF5_SPI2           5
#define GPIO_AF5_SPI5           5
#define GPIO_AF5_I2S1           5
#define GPIO_AF5_I2S2           5
#define GPIO_AF5_I2S3           5
#define GPIO_AF5_I2S_CKIN       5
#define GPIO_AF6_SPI3           6
#define GPIO_AF6_I2S3           6
#define GPIO_AF6_SAI1           6
#define GPIO_AF7_USART1         7
#define GPIO_AF8_UART7          8
#define GPIO_AF8_SAI2           8
#define GPIO_AF10_USB_FS        10
#define GPIO_AF10_USB_HS        10
#define GPIO_AF10_SAI2          10
#define GPIO_AF11_ETH           11
#define GPIO_AF12_FMC           12
#define GPIO_AF12_SDMMC1        12
#define GPIO_AF12_USB_FS        12
#define GPIO_AF13_DCMI          13

#define GPIOCFG_MODE_SHIFT      4
#define GPIOCFG_MODE_MASK      (3 << GPIOCFG_MODE_SHIFT)
#define GPIOCFG_MODE_INP       (0 << GPIOCFG_MODE_SHIFT)    // 00: Input (reset state)
#define GPIOCFG_MODE_OUT       (1 << GPIOCFG_MODE_SHIFT)    // 01: General purpose output mode
#define GPIOCFG_MODE_ALT       (2 << GPIOCFG_MODE_SHIFT)    // 10: Alternate function mode
#define GPIOCFG_MODE_ANA       (3 << GPIOCFG_MODE_SHIFT)    // 11: Analog mode
#define GPIOCFG_OTYPE_SHIFT     6
#define GPIOCFG_OTYPE_MASK     (1 << GPIOCFG_OTYPE_SHIFT)
#define GPIOCFG_OTYPE_PUPD     (0 << GPIOCFG_OTYPE_SHIFT)   // 0: Output push-pull (reset state)
#define GPIOCFG_OTYPE_OPEN     (1 << GPIOCFG_OTYPE_SHIFT)   // 1: Output open-drain
#define GPIOCFG_OSPEED_SHIFT    7
#define GPIOCFG_OSPEED_MASK    (3 << GPIOCFG_OSPEED_SHIFT)
#define GPIOCFG_OSPEED_LOW     (0 << GPIOCFG_OSPEED_SHIFT)  // 00: Low speed       (   2 MHz)
#define GPIOCFG_OSPEED_MEDIUM  (1 << GPIOCFG_OSPEED_SHIFT)  // 01: Medium speed    (  25 MHz)
#define GPIOCFG_OSPEED_HIGH    (2 << GPIOCFG_OSPEED_SHIFT)  // 10: High speed      (  50 MHz)
#define GPIOCFG_OSPEED_VHIGH   (3 << GPIOCFG_OSPEED_SHIFT)  // 11: Very high speed ( 100 Mhz)
#define GPIOCFG_PUPD_SHIFT      9
#define GPIOCFG_PUPD_MASK      (3 << GPIOCFG_PUPD_SHIFT)
#define GPIOCFG_PUPD_NONE      (0 << GPIOCFG_PUPD_SHIFT)    // 00: No pull-up, pull-down
#define GPIOCFG_PUPD_PUP       (1 << GPIOCFG_PUPD_SHIFT)    // 01: Pull-up
#define GPIOCFG_PUPD_PDN       (2 << GPIOCFG_PUPD_SHIFT)    // 10: Pull-down
#define GPIOCFG_PUPD_RFU       (3 << GPIOCFG_PUPD_SHIFT)    // 11: Reserved

// IRQ triggers
#define GPIO_IRQ_MASK           0x38
#define GPIO_IRQ_FALLING        0x20
#define GPIO_IRQ_RISING         0x28
#define GPIO_IRQ_CHANGE         0x30

// configure operation mode of GPIO pin
void hw_cfg_pin(GPIO_TypeDef* gpioport, uint8_t pin, uint16_t gpiocfg);

// set state of GPIO output pin
void hw_set_pin(GPIO_TypeDef* gpioport, uint8_t pin, uint8_t state);

// get state of GPIO input pin
uint8_t hw_get_pin(GPIO_TypeDef* gpioport, uint8_t pin);

// configure given line as external interrupt source (EXTI handler)
void hw_cfg_extirq(uint8_t portidx, uint8_t pin, uint8_t irqcfg, uint8_t irqpr);

#endif // STM32F7XX_HW_H_