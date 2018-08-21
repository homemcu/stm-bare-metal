/*
* This file is a compilation stm8s.h from
* "Tiny peripheral library for STM8S" by lujji
* https://github.com/lujji/stm8-bare-min/blob/master/stm8/stm8s.h
* and stm8as.h from
* "STM8 Libraries and Examples with Arduino look&feel" by Georg Icking-Konert
* date 2017-02-11, version 0.3, copyright MIT License (MIT)
* https://github.com/gicking/STM8_templates/blob/master/Library/Base/inc/stm8as.h
*
* Many thanks to both of them for their excellent work.
*/
#ifndef STM8_H
#define STM8_H

/* Supported STM8 devices. Define respective device in Makefile
 STM8S103     standard line low density device
 STM8S903     standard line low density device
 STM8S105     standard line medium density device
 STM8S208     standard line high density device with CAN
 STM8S207     standard line high density device without CAN
 STM8AF622x   automotive low density devices without CAN
 STM8AF626x   automotive medium density devices without CAN
 STM8AF52Ax   automotive high density devices with CAN
 STM8AF62Ax   automotive high density devices without CAN
 STM8S003     value line low density device
 STM8S005     value line medium density device
 STM8S007     value line high density device
*/

// check if device is supported
#if !defined (STM8S103) && !defined (STM8S903) && !defined (STM8S105) && !defined (STM8S208) \
    && !defined (STM8S207) && !defined (STM8AF622x) && !defined (STM8AF626x) && !defined (STM8AF52Ax) \
    && !defined (STM8AF62Ax) && !defined (STM8S003) && !defined (STM8S005) && !defined (STM8S007) 
  #error "select supported STM8S/A device. For a list, see file 'stm8.h'"
#endif

#define _MEM_(mem_addr)         (*(volatile uint8_t *)(mem_addr))
#define _SFR_(mem_addr)         (*(volatile uint8_t *)(0x5000 + (mem_addr)))

/* PORT A */
#define PA_ODR                  _SFR_(0x00)
#define PA_IDR                  _SFR_(0x01)
#define PA_DDR                  _SFR_(0x02)
#define PA_CR1                  _SFR_(0x03)
#define PA_CR2                  _SFR_(0x04)
/* PORT B */
#define PB_ODR                  _SFR_(0x05)
#define PB_IDR                  _SFR_(0x06)
#define PB_DDR                  _SFR_(0x07)
#define PB_CR1                  _SFR_(0x08)
#define PB_CR2                  _SFR_(0x09)
/* PORT C */
#define PC_ODR                  _SFR_(0x0A)
#define PC_IDR                  _SFR_(0x0B)
#define PC_DDR                  _SFR_(0x0C)
#define PC_CR1                  _SFR_(0x0D)
#define PC_CR2                  _SFR_(0x0E)
/* PORT D */
#define PD_ODR                  _SFR_(0x0F)
#define PD_IDR                  _SFR_(0x10)
#define PD_DDR                  _SFR_(0x11)
#define PD_CR1                  _SFR_(0x12)
#define PD_CR2                  _SFR_(0x13)
/* PORT E */
#define PE_ODR                  _SFR_(0x14)
#define PE_IDR                  _SFR_(0x15)
#define PE_DDR                  _SFR_(0x16)
#define PE_CR1                  _SFR_(0x17)
#define PE_CR2                  _SFR_(0x18)
/* PORT F */
#define PF_ODR                  _SFR_(0x19)
#define PF_IDR                  _SFR_(0x1A)
#define PF_DDR                  _SFR_(0x1B)
#define PF_CR1                  _SFR_(0x1C)
#define PF_CR2                  _SFR_(0x1D)

#if defined(STM8S207) || defined (STM8S007) || defined(STM8S208) || defined(STM8S105) || \
    defined(STM8S005) || defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
/* PORT G */
#define PG_ODR                  _SFR_(0x1E)
#define PG_IDR                  _SFR_(0x1F)
#define PG_DDR                  _SFR_(0x20)
#define PG_CR1                  _SFR_(0x21)
#define PG_CR2                  _SFR_(0x22)
#endif

#if defined(STM8S207) || defined (STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/* PORT H */
#define PH_ODR                  _SFR_(0x23)
#define PH_IDR                  _SFR_(0x24)
#define PH_DDR                  _SFR_(0x25)
#define PH_CR1                  _SFR_(0x26)
#define PH_CR2                  _SFR_(0x27)
/* PORT I */
#define PI_ODR                  _SFR_(0x28)
#define PI_IDR                  _SFR_(0x29)
#define PI_DDR                  _SFR_(0x2A)
#define PI_CR1                  _SFR_(0x2B)
#define PI_CR2                  _SFR_(0x2C)
#endif

/* Flash */
#define FLASH_CR1               _SFR_(0x5A)
#define FLASH_CR1_HALT          3
#define FLASH_CR1_AHALT         2
#define FLASH_CR1_IE            1
#define FLASH_CR1_FIX           0
#define FLASH_CR2               _SFR_(0x5B)
#define FLASH_CR2_OPT           7
#define FLASH_CR2_WPRG          6
#define FLASH_CR2_ERASE         5
#define FLASH_CR2_FPRG          4
#define FLASH_CR2_PRG           0
#define FLASH_NCR2              _SFR_(0x5C)
#define FLASH_NCR2_NOPT         7
#define FLASH_NCR2_NWPRG        6
#define FLASH_NCR2_NERASE       5
#define FLASH_NCR2_NFPRG        4
#define FLASH_NCR2_NPRG         0
#define FLASH_FPR               _SFR_(0x5D)
#define FLASH_NFPR              _SFR_(0x5E)
#define FLASH_IAPSR             _SFR_(0x5F)
#define FLASH_IAPSR_DUL         3
#define FLASH_IAPSR_EOP         2
#define FLASH_IAPSR_PUL         1
#define FLASH_PUKR              _SFR_(0x62)
#define FLASH_PUKR_KEY1         0x56
#define FLASH_PUKR_KEY2         0xAE
#define FLASH_DUKR              _SFR_(0x64)
#define FLASH_DUKR_KEY1         FLASH_PUKR_KEY2
#define FLASH_DUKR_KEY2         FLASH_PUKR_KEY1

/* ITC */

/* CLK */
#define CLK_ICKR                _SFR_(0xC0)
#define CLK_ECKR                _SFR_(0xC1)
#define CLK_ECKR_HSERDY         1
#define CLK_ECKR_HSEEN          0
#define CLK_CMSR                _SFR_(0xC3)
#define CLK_SWR                 _SFR_(0xC4)
#define CLK_SWCR                _SFR_(0xC5)
#define CLK_SWCR_SWIF           3
#define CLK_SWCR_SWIEN          2
#define CLK_SWCR_SWEN           1
#define CLK_SWCR_SWBSY          0
#define CLK_CKDIVR              _SFR_(0xC6)
#define CLK_PCKENR1             _SFR_(0xC7)
#define CLK_CSSR                _SFR_(0xC8)
#define CLK_CCOR                _SFR_(0xC9)
#define CLK_CCOR_CCOEN          0
#define CLK_PCKENR2             _SFR_(0xCA)
#define CLK_HSITRIMR            _SFR_(0xCC)
#define CLK_SWIMCCR             _SFR_(0xCD)

/* WWDG */
#define WWDG_CR                 _SFR_(0xD1)
#define WWDG_WR                 _SFR_(0xD2)

/* IWDG */
#define IWDG_KR                 _SFR_(0xE0)
#define IWDG_KEY_ENABLE         0xCC
#define IWDG_KEY_REFRESH        0xAA
#define IWDG_KEY_ACCESS         0x55
#define IWDG_PR                 _SFR_(0xE1)
#define IWDG_RLR                _SFR_(0xE2)

/* AWU */

/* BEEP */

/* SPI */
#define SPI_CR1                 _SFR_(0x200)
#define SPI_CR1_LSBFIRST        7
#define SPI_CR1_SPE             6
#define SPI_CR1_BR2             5
#define SPI_CR1_BR1             4
#define SPI_CR1_BR0             3
#define SPI_CR1_MSTR            2
#define SPI_CR1_CPOL            1
#define SPI_CR1_CPHA            0
#define SPI_CR2                 _SFR_(0x201)
#define SPI_CR2_BDM             7
#define SPI_CR2_BDOE            6
#define SPI_CR2_CRCEN           5
#define SPI_CR2_CRCNEXT         4
#define SPI_CR2_RXONLY          2
#define SPI_CR2_SSM             1
#define SPI_CR2_SSI             0
#define SPI_ICR                 _SFR_(0x202)
#define SPI_SR                  _SFR_(0x203)
#define SPI_SR_BSY              7
#define SPI_SR_OVR              6
#define SPI_SR_MODF             5
#define SPI_SR_CRCERR           4
#define SPI_SR_WKUP             3
#define SPI_SR_TXE              1
#define SPI_SR_RXNE             0
#define SPI_DR                  _SFR_(0x204)
#define SPI_CRCPR               _SFR_(0x205)
#define SPI_RXCRCR              _SFR_(0x206)
#define SPI_TXCRCR              _SFR_(0x207)

/* I2C */
#define I2C_CR1                 _SFR_(0x210)
#define I2C_CR1_PE              0
#define I2C_CR2                 _SFR_(0x211)
#define I2C_CR2_ACK             2
#define I2C_CR2_STOP            1
#define I2C_CR2_START           0
#define I2C_FREQR               _SFR_(0x212)
#define I2C_FREQR_FREQ4         4
#define I2C_FREQR_FREQ3         3
#define I2C_FREQR_FREQ2         2
#define I2C_FREQR_FREQ1         1
#define I2C_FREQR_FREQ0         0
#define I2C_OARL                _SFR_(0x213)
#define I2C_OARH                _SFR_(0x214)
#define I2C_OARH_ADDMODE        7
#define I2C_OARH_ADDCONF        6
#define I2C_DR                  _SFR_(0x216)
#define I2C_SR1                 _SFR_(0x217)
#define I2C_SR1_TXE             7
#define I2C_SR1_RXNE            6
#define I2C_SR1_BTF             2
#define I2C_SR1_ADDR            1
#define I2C_SR1_SB              0
#define I2C_SR2                 _SFR_(0x218)
#define I2C_SR3                 _SFR_(0x219)
#define I2C_SR3_BUSY            1
#define I2C_SR3_MSL             0
#define I2C_ITR                 _SFR_(0x21A)
#define I2C_CCRL                _SFR_(0x21B)
#define I2C_CCRH                _SFR_(0x21C)
#define I2C_TRISER              _SFR_(0x21D)
#define I2C_PECR                _SFR_(0x21E)

#if defined(STM8S208) ||defined(STM8S207) || defined (STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/* UART1 */
#define UART1_SR                _SFR_(0x230)
#define UART1_SR_TXE            7
#define UART1_SR_TC             6
#define UART1_SR_RXNE           5
#define UART1_DR                _SFR_(0x231)
#define UART1_BRR1              _SFR_(0x232)
#define UART1_BRR2              _SFR_(0x233)
#define UART1_CR1               _SFR_(0x234)
#define UART1_CR2               _SFR_(0x235)
#define UART1_CR2_TEN           3
#define UART1_CR2_REN           2
#define UART1_CR3               _SFR_(0x236)
#define UART1_CR4               _SFR_(0x237)
#define UART1_CR5               _SFR_(0x238)
#define UART1_GTR               _SFR_(0x239)
#define UART1_PSCR              _SFR_(0x23A)
#endif

#if defined (STM8S105) || defined (STM8S005) || defined (STM8AF626x)
/* UART2 */
#define UART2_SR                _SFR_(0x240)
#define UART2_SR_TXE            7
#define UART2_SR_TC             6
#define UART2_SR_RXNE           5
#define UART2_DR                _SFR_(0x241)
#define UART2_BRR1              _SFR_(0x242)
#define UART2_BRR2              _SFR_(0x243)
#define UART2_CR1               _SFR_(0x244)
#define UART2_CR2               _SFR_(0x245)
#define UART2_CR2_TEN           3
#define UART2_CR2_REN           2
#define UART2_CR3               _SFR_(0x246)
#define UART2_CR4               _SFR_(0x247)
#define UART2_CR5               _SFR_(0x248)
#define UART2_CR6               _SFR_(0x249)
#define UART2_GTR               _SFR_(0x24A)
#define UART2_PSCR              _SFR_(0x24B)
#endif

#if defined(STM8S208) ||defined(STM8S207) || defined (STM8S007) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/* UART3 */
#endif

#if defined(STM8AF622x)
/* UART4 */
#endif

/* TIM1 */
#define TIM1_CR1                _SFR_(0x250)
#define TIM1_CR2                _SFR_(0x251)
#define TIM1_SMCR               _SFR_(0x252)
#define TIM1_ETR                _SFR_(0x253)
#define TIM1_IER                _SFR_(0x254)
#define TIM1_SR1                _SFR_(0x255)
#define TIM1_SR2                _SFR_(0x256)
#define TIM1_EGR                _SFR_(0x257)
#define TIM1_CCMR1              _SFR_(0x258)
#define TIM1_CCMR2              _SFR_(0x259)
#define TIM1_CCMR3              _SFR_(0x25A)
#define TIM1_CCMR4              _SFR_(0x25B)
#define TIM1_CCER1              _SFR_(0x25C)
#define TIM1_CCER2              _SFR_(0x25D)
#define TIM1_CNTRH              _SFR_(0x25E)
#define TIM1_CNTRL              _SFR_(0x25F)
#define TIM1_PSCRH              _SFR_(0x260)
#define TIM1_PSCRL              _SFR_(0x261)
#define TIM1_ARRH               _SFR_(0x262)
#define TIM1_ARRL               _SFR_(0x263)
#define TIM1_RCR                _SFR_(0x264)
#define TIM1_CCR1H              _SFR_(0x265)
#define TIM1_CCR1L              _SFR_(0x266)
#define TIM1_CCR2H              _SFR_(0x267)
#define TIM1_CCR2L              _SFR_(0x268)
#define TIM1_CCR3H              _SFR_(0x269)
#define TIM1_CCR3L              _SFR_(0x26A)
#define TIM1_CCR4H              _SFR_(0x26B)
#define TIM1_CCR4L              _SFR_(0x26C)
#define TIM1_BKR                _SFR_(0x26D)
#define TIM1_DTR                _SFR_(0x26E)
#define TIM1_OISR               _SFR_(0x26F)

#if defined(STM8S208) || defined(STM8S207) || defined (STM8S007) || defined(STM8S103) || defined(STM8S003) || \
    defined(STM8S105) || defined(STM8S005) || defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
/* TIM2 */
#define TIM2_CR1                _SFR_(0x300)
#if defined(STM8S103) || defined(STM8S003)
#define TIM2_IER                _SFR_(0x303)
#define TIM2_SR1                _SFR_(0x304)
#define TIM2_SR2                _SFR_(0x305)
#define TIM2_EGR                _SFR_(0x306)
#define TIM2_CCMR1              _SFR_(0x307)
#define TIM2_CCMR2              _SFR_(0x308)
#define TIM2_CCMR3              _SFR_(0x309)
#define TIM2_CCER1              _SFR_(0x30A)
#define TIM2_CCER2              _SFR_(0x30B)
#define TIM2_CNTRH              _SFR_(0x30C)
#define TIM2_CNTRL              _SFR_(0x30D)
#define TIM2_PSCR               _SFR_(0x30E)
#define TIM2_ARRH               _SFR_(0x30F)
#define TIM2_ARRL               _SFR_(0x310)
#define TIM2_CCR1H              _SFR_(0x311)
#define TIM2_CCR1L              _SFR_(0x312)
#define TIM2_CCR2H              _SFR_(0x313)
#define TIM2_CCR2L              _SFR_(0x314)
#define TIM2_CCR3H              _SFR_(0x315)
#define TIM2_CCR3L              _SFR_(0x316)
#else
#define TIM2_IER                _SFR_(0x301)
#define TIM2_SR1                _SFR_(0x302)
#define TIM2_SR2                _SFR_(0x303)
#define TIM2_EGR                _SFR_(0x304)
#define TIM2_CCMR1              _SFR_(0x305)
#define TIM2_CCMR2              _SFR_(0x306)
#define TIM2_CCMR3              _SFR_(0x307)
#define TIM2_CCER1              _SFR_(0x308)
#define TIM2_CCER2              _SFR_(0x309)
#define TIM2_CNTRH              _SFR_(0x30A)
#define TIM2_CNTRL              _SFR_(0x30B)
#define TIM2_PSCR               _SFR_(0x30C)
#define TIM2_ARRH               _SFR_(0x30D)
#define TIM2_ARRL               _SFR_(0x30E)
#define TIM2_CCR1H              _SFR_(0x30F)
#define TIM2_CCR1L              _SFR_(0x310)
#define TIM2_CCR2H              _SFR_(0x311)
#define TIM2_CCR2L              _SFR_(0x312)
#define TIM2_CCR3H              _SFR_(0x313)
#define TIM2_CCR3L              _SFR_(0x314)
#endif
#endif

#if defined(STM8S208) || defined(STM8S207) || defined (STM8S007) || defined(STM8S105) || \
    defined(STM8S005) || defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
/* TIM3 */
#define TIM3_CR1                _SFR_(0x320)
#define TIM3_CR1_CEN            0
#define TIM3_IER                _SFR_(0x321)
#define TIM3_IER_UIE            0
#define TIM3_SR1                _SFR_(0x322)
#define TIM3_SR1_UIF            0
#define TIM3_SR2                _SFR_(0x323)
#define TIM3_EGR                _SFR_(0x324)
#define TIM3_CCMR1              _SFR_(0x325)
#define TIM3_CCMR2              _SFR_(0x326)
#define TIM3_CCER1              _SFR_(0x327)
#define TIM3_CNTRH              _SFR_(0x328)
#define TIM3_CNTRL              _SFR_(0x329)
#define TIM3_PSCR               _SFR_(0x32A)
#define TIM3_ARRH               _SFR_(0x32B)
#define TIM3_ARRL               _SFR_(0x32C)
#define TIM3_CCR1H              _SFR_(0x32D)
#define TIM3_CCR1L              _SFR_(0x32E)
#define TIM3_CCR2H              _SFR_(0x32F)
#define TIM3_CCR2L              _SFR_(0x330)
#endif

#if defined(STM8S208) ||defined(STM8S207) || defined (STM8S007) || defined(STM8S103) || defined(STM8S003) || \
    defined(STM8S105) || defined(STM8S005) || defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
/* TIM4 */
#define TIM4_CR1                _SFR_(0x340)
#define TIM4_CR1_ARPE           7
#define TIM4_CR1_URS            2
#define TIM4_CR1_CEN            0
#if defined(STM8S103) || defined(STM8S003)
#define TIM4_IER                _SFR_(0x343)
#define TIM4_IER_UIE            0
#define TIM4_SR                 _SFR_(0x344)
#define TIM4_SR_UIF             0
#define TIM4_EGR                _SFR_(0x345)
#define TIM4_CNTR               _SFR_(0x346)
#define TIM4_PSCR               _SFR_(0x347)
#define TIM4_ARR                _SFR_(0x348)
#else
#define TIM4_IER                _SFR_(0x341)
#define TIM4_IER_UIE            0
#define TIM4_SR                 _SFR_(0x342)
#define TIM4_SR_UIF             0
#define TIM4_EGR                _SFR_(0x343)
#define TIM4_CNTR               _SFR_(0x344)
#define TIM4_PSCR               _SFR_(0x345)
#define TIM4_ARR                _SFR_(0x346)
#endif
#endif

#if defined (STM8S903) || defined (STM8AF622x)
/* TIM5 */
#endif
#if defined (STM8S903) || defined (STM8AF622x)
/* TIM6 */
#endif

#if defined(STM8S105) || defined(STM8S005) || defined(STM8S103) || defined(STM8S003) || \
    defined(STM8S903) || defined(STM8AF626x) || defined(STM8AF622x)
/* ADC1 */
#define ADC1_DB0R               _SFR_(0x3E0)
#define ADC1_CSR                _SFR_(0x400)
#define ADC1_CSR_EOC            7
#define ADC1_CSR_AWD            6
#define ADC1_CSR_EOCIE          5
#define ADC1_CSR_CH3            3
#define ADC1_CSR_CH2            2
#define ADC1_CSR_CH1            1
#define ADC1_CSR_CH0            0
#define ADC1_CR1                _SFR_(0x401)
#define ADC1_CR1_SPSEL2         6
#define ADC1_CR1_SPSEL1         5
#define ADC1_CR1_SPSEL0         4
#define ADC1_CR1_CONT           1
#define ADC1_CR1_ADON           0
#define ADC1_CR2                _SFR_(0x402)
#define ADC1_CR2_EXTTRIG        6
#define ADC1_CR2_EXTSEL1        5
#define ADC1_CR2_EXTSEL0        4
#define ADC1_CR2_ALIGN          3
#define ADC1_CR2_SCAN           1
#define ADC1_CR3                _SFR_(0x403)
#define ADC1_CR3_DBUF           7
#define ADC1_CR3_OVR            6
#define ADC1_DRH                _SFR_(0x404)
#define ADC1_DRL                _SFR_(0x405)
#define ADC1_TDRH               _SFR_(0x406)
#define ADC1_TDRL               _SFR_(0x407)
#define ADC1_HTRH               _SFR_(0x408)
#define ADC1_HTRL               _SFR_(0x409)
#define ADC1_LTRH               _SFR_(0x40A)
#define ADC1_LTRL               _SFR_(0x40B)
#define ADC1_AWSRH              _SFR_(0x40C)
#define ADC1_AWSRL              _SFR_(0x40D)
#define ADC1_AWCRH              _SFR_(0x40E)
#define ADC1_AWCRL              _SFR_(0x40F)
#endif

#if defined(STM8S208) || defined(STM8S207) || defined (STM8S007) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/* ADC2 */
#endif

#if defined (STM8S208) || defined (STM8AF52Ax)
/* CAN */
#endif


/* Interrupts */

// trap - Software interrupt (call TRIGGER_TRAP to trip)
#define TLI_ISR                 0

// irq1 - Auto Wake Up from Halt interrupt (AWU)
#define AWU_ISR                 1

// irq2 - Clock Controller interrupt
#define CLK_ISR                 2

// irq3 - External interrupt 0 (GPIOA)
#define PORTA_ISR               3

// irq4 - External interrupt 1 (GPIOB)
#define PORTB_ISR               4

// irq5 - External interrupt 2 (GPIOC)
#define PORTC_ISR               5

// irq6 - External interrupt 3 (GPIOD)
#define PORTD_ISR               6

// irq7 - External interrupt 4 (GPIOE)
#define PORTE_ISR               7

// irq8 - External interrupt 5 (GPIOF) or CAN receive interrupt -- device dependent
#ifdef STM8S903
#define PORTF_ISR               8
#elif defined (STM8S208) || defined (STM8AF52Ax)
#define CAN_RX_ISR              8
#endif

// irq9 - CAN transmit interrupt -- device dependent
#if defined (STM8S208) || defined (STM8AF52Ax)
#define CAN_TX_ISR              9
#endif

// irq10 - SPI End of transfer interrupt
#define SPI_ISR                 10

// irq11 - TIM1 Update/Overflow/Trigger/Break interrupt
#define TIM1_OVF_ISR            11

// irq12 - TIM1 Capture/Compare interrupt
#define TIM1_CC_ISR             12

// irq13 - TIM5 or TIM2 Update/overflow interrupt -- device dependent
#ifdef STM8S903
#define TIM5_OVF_ISR            13
#else
#define TIM2_OVF_ISR            13
#endif

// irq14 - TIM5 or TIM2 Capture/Compare interrupt -- device dependent
#ifdef STM8S903
#define TIM5_CC_ISR             14
#else
#define TIM2_CC_ISR             14
#endif

// irq15 - TIM3 Update/overflow interrupt -- device dependent
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
#define TIM3_OVF_ISR            15
#endif

// irq16 - TIM3 Capture/Compare interrupt -- device dependent
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
#define TIM3_CC_ISR             16
#endif

// irq17 - USART/UART1 send (TX empty) interrupt -- device dependent
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
#define UART1_TXC_ISR           17
#endif

// irq18 - USART/UART1 receive (RX full) interrupt -- device dependent
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
#define UART1_RXC_ISR           18
#endif

// irq19 - I2C interrupt
#define I2C_ISR                 19

// irq20 - UART2/3/4 send (TX empty) interrupt -- device dependent
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x) || defined(STM8S207) || \
    defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
#define UART234_TXC_ISR         20
#endif

// irq21 - UART2/3/4 receive (RX full) interrupt -- device dependent
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x) || defined(STM8S207) || \
    defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
#define UART234_RXC_ISR         21
#endif

// irq22 - ADC1/2 end of conversion/Analog watchdog interrupts
#define ADC1_ISR                22

// irq23 - TIM4 or TIM6 Update/Overflow interrupt
#if defined(STM8S903) || defined(STM8AF622x)
#define TIM6_ISR                23
#else
#define TIM4_ISR                23
#endif

// irq24 - flash interrupt
#define FLASH_ISR               24

#endif /* STM8_H */
