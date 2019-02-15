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

#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include "compiler-def.h"
#include "stm32f746xx.h"

#ifndef BYTE_ORDER
#define BYTE_ORDER       LITTLE_ENDIAN
#endif 

#define CPU_TO_LE16(x)   (x)
#define CPU_TO_LE32(x)   (x)
#define cpu_to_le16(x)   (x)
#define cpu_to_le32(x)   (x)

static inline uint32_t cpu_to_le32p(const uint32_t *p)
{
	return (uint32_t)*p;
}
static inline uint16_t cpu_to_le16p(const uint16_t *p)
{
	return (uint16_t)*p;
}
static inline uint32_t le32_to_cpup(const uint32_t *p)
{
	return (uint32_t)*p;
}
static inline uint16_t le16_to_cpup(const uint16_t *p)
{
	return (uint16_t)*p;
}

#endif // CPU_H_
