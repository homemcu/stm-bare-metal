/*
* Copyright (c) 2019, 2020 Vladimir Alemasov
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

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

#if defined USBD_OTGFS || defined USBD_DEVFS || defined USBD_OTGHS_FS
#define USBD_FULL_SPEED
#endif

#define USBD_SOF_DISABLED

#ifdef USBD_ULPI
#define USBD_VBUS_DETECT
#endif

#if defined STM32F746xx && defined USBD_OTGFS
#define USBD_VBUS_DETECT
#endif

// 1, 2?(not tested yet)
#define I2S_CHANNELS      1
#ifdef USBD_FULL_SPEED
// 44100, 48000
#define I2S_FCLK          48000
// 24
#define I2S_BITRES        24
// 16, 32
#define UAC_BITRES        16
#else
// 44100, 48000
#define I2S_FCLK          48000
// 24
#define I2S_BITRES        24
// 16, 32
#define UAC_BITRES        32
#endif

#endif /* PROJECT_CONF_H_ */