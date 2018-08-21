#ifndef COMPILER_DEF_H_
#define COMPILER_DEF_H_

//-----------------------------------------------------------------------------
// GNU ARM Embedded
// https://launchpad.net/~team-gcc-arm-embedded/+archive/ubuntu/ppa
#if defined __GNUC__ && !defined __CC_ARM

#define DECLARE_ALIGNED_4(x) x __attribute__((aligned(4)))

//-----------------------------------------------------------------------------
// IAR STM32
// http://www.iar.com
#elif defined __ICCARM__

#define DECLARE_ALIGNED_4(x) _Pragma("data_alignment=4") x

#define FLASH_MEMORY_DECLARE(type, name) type const name
#define FLASH_MEMORY_READ_BYTE(byte) *(byte)

#else
#error "Unsupported compiler."

#endif

#endif // COMPILER_DEF_H_
