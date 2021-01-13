/*
* Copyright (c) 2019-2021 Vladimir Alemasov
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
#include "usbd_core.h"
#include "usb_std.h"

//--------------------------------------------
#ifdef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
// = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY in FreeRTOSConfig.h
#define OTG_FS_IRQ_PREEMPT_PRIORITY     (configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1)
#define OTG_HS_IRQ_PREEMPT_PRIORITY     (configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1)
#else
#define OTG_FS_IRQ_PREEMPT_PRIORITY     14
#define OTG_HS_IRQ_PREEMPT_PRIORITY     14
#endif

//--------------------------------------------
static usbd_device *udev;

#ifdef USBD_OTGFS
//--------------------------------------------
// USB_FS(CLK48)
// GPIO_AF10_USB_FS
// SOF:   PA8
// ID:    PA10
// DM:    PA11
// DP:    PA12
// VBUS:  PA9, a 5 V-tolerant pin, is natively dedicated to VBUS sensing.

//--------------------------------------------
#define	PORT_SOF        GPIO_A
#define	PIN_SOF         8
#define	PORT_DM         GPIO_A
#define	PIN_DM          11
#define	PORT_DP         GPIO_A
#define	PIN_DP          12

//--------------------------------------------
void usbd_hw_init(usbd_device *dev)
{
	// IO port A clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

#ifdef USBD_SOF_OUT
	hw_cfg_pin(GPIOx(PORT_SOF), PIN_SOF, GPIOCFG_MODE_ALT | GPIO_AF10_USB_FS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
#endif
	hw_cfg_pin(GPIOx(PORT_DM),  PIN_DM,  GPIOCFG_MODE_ALT | GPIO_AF10_USB_FS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_DP),  PIN_DP,  GPIOCFG_MODE_ALT | GPIO_AF10_USB_FS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);

	NVIC_SetPriority(OTG_FS_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), OTG_FS_IRQ_PREEMPT_PRIORITY, 0));
	NVIC_EnableIRQ(OTG_FS_IRQn);

	udev = dev;
}

//--------------------------------------------
// OTG FS interruption handler
void OTG_FS_IRQHandler(void)
{
	usbd_poll(udev);
}

#elif defined USBD_OTGHS || defined USBD_OTGHS_FS

#ifdef USBD_ULPI
//--------------------------------------------
// USBD_ULPI(CLK48)
// GPIO_AF10_USB_HS
// ULPI_D0:    PA3
// ULPI_D1:    PB0
// ULPI_D2:    PB1
// ULPI_D3:    PB10
// ULPI_D4:    PB11
// ULPI_D5:    PB12
// ULPI_D6:    PB13
// ULPI_D7:    PB5
// ULPI_CK:    PA5
// ULPI_STP:   PC0
// ULPI_DIR:   PC2    PI11
// ULPI_NXT:   PC3    PH4

//--------------------------------------------
#define	PORT_D0         GPIO_A // PA3  <-> DATA0
#define	PIN_D0          3
#define	PORT_D1         GPIO_B // PB0  <-> DATA1
#define	PIN_D1          0
#define	PORT_D2         GPIO_B // PB1  <-> DATA2
#define	PIN_D2          1
#define	PORT_D3         GPIO_B // PB10 <-> DATA3
#define	PIN_D3          10
#define	PORT_D4         GPIO_B // PB11 <-> DATA4
#define	PIN_D4          11
#define	PORT_D5         GPIO_B // PB12 <-> DATA5
#define	PIN_D5          12
#define	PORT_D6         GPIO_B // PB13 <-> DATA6
#define	PIN_D6          13
#define	PORT_D7         GPIO_B // PB5  <-> DATA7
#define	PIN_D7          5
#define	PORT_CLK        GPIO_A // PA5  <-- CLK
#define	PIN_CLK         5
#define	PORT_STP        GPIO_C // PC0  --> STP
#define	PIN_STP         0
#define	PORT_DIR        GPIO_C // PC2  <-- DIR
#define	PIN_DIR         2
#define	PORT_NXT        GPIO_C // PC3  <-- NXT
#define	PIN_NXT         3

#else
//--------------------------------------------
// USB_FS(CLK48)
// GPIO_AF12_USB_FS
// SOF:   PA4
// ID:    PB12
// DM:    PB14
// DP:    PB15
// VBUS:  PA9, a 5 V-tolerant pin, is natively dedicated to VBUS sensing.

//--------------------------------------------
#define	PORT_SOF        GPIO_A
#define	PIN_SOF         4
#define	PORT_DM         GPIO_B
#define	PIN_DM          14
#define	PORT_DP         GPIO_B
#define	PIN_DP          15

#endif

//--------------------------------------------
void usbd_hw_init(usbd_device *dev)
{
#ifdef USBD_ULPI
	// IO port A clock enable
	// IO port B clock enable
	// IO port C clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
#else
#ifdef USBD_SOF_OUT
	// IO port A clock enable
	// IO port B clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
#else
	// IO port B clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
#endif
#endif

#ifdef USBD_ULPI
	hw_cfg_pin(GPIOx(PORT_D0),    PIN_D0,    GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D1),    PIN_D1,    GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D2),    PIN_D2,    GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D3),    PIN_D3,    GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D4),    PIN_D4,    GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D5),    PIN_D5,    GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D6),    PIN_D6,    GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_D7),    PIN_D7,    GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_CLK),   PIN_CLK,   GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_STP),   PIN_STP,   GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_DIR),   PIN_DIR,   GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_NXT),   PIN_NXT,   GPIOCFG_MODE_ALT | GPIO_AF10_USB_HS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
#else
#ifdef USBD_SOF_OUT
	hw_cfg_pin(GPIOx(PORT_SOF), PIN_SOF, GPIOCFG_MODE_ALT | GPIO_AF12_USB_FS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
#endif
	hw_cfg_pin(GPIOx(PORT_DM),  PIN_DM,  GPIOCFG_MODE_ALT | GPIO_AF12_USB_FS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
	hw_cfg_pin(GPIOx(PORT_DP),  PIN_DP,  GPIOCFG_MODE_ALT | GPIO_AF12_USB_FS | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_NONE);
#endif

	NVIC_SetPriority(OTG_HS_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), OTG_FS_IRQ_PREEMPT_PRIORITY, 0));
	NVIC_EnableIRQ(OTG_HS_IRQn);

	udev = dev;
}

//--------------------------------------------
// OTG HS interruption handler
void OTG_HS_IRQHandler(void)
{
	usbd_poll(udev);
}
#endif
