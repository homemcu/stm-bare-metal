#ifndef COMPILER_DEF_H_
#define COMPILER_DEF_H_

//-----------------------------------------------------------------------------
// GNU ARM Embedded
// https://launchpad.net/~team-gcc-arm-embedded/+archive/ubuntu/ppa
#if defined __GNUC__ && !defined __CC_ARM

#define DECLARE_ALIGNED_4(x) x __attribute__((aligned(4)))

#define FLASH_MEMORY_DECLARE(type, name) type const name
#define FLASH_MEMORY_READ_BYTE(byte) *(byte)

#ifndef __packed
#define __packed __attribute__((__packed__))
#endif

//-----------------------------------------------------------------------------
// IAR STM32
// http://www.iar.com
#elif defined __ICCARM__

#define DECLARE_ALIGNED_4(x) _Pragma("data_alignment=4") x

#define FLASH_MEMORY_DECLARE(type, name) type const name
#define FLASH_MEMORY_READ_BYTE(byte) *(byte)

#if __VER__ <= 7080003
// In IAR EW ARM version 7.80 and earlier, the size of wchar_t is two bytes.
#define u L
#endif

#else
#error "Unsupported compiler."

#endif

#endif // COMPILER_DEF_H_
