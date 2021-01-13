











 




















 














 


 
 

  #pragma system_include

 
 

 

  #pragma system_include














 


 
 


  #pragma system_include

 



 

 

 

 
#pragma rtmodel = "__dlib_version", "6"

 


 



























 


  #pragma system_include

 
 
 


  #pragma system_include

 

   

 
 


   #pragma system_include






 




 


 


 


 

 


 

 

 

 

 

 

 

 

 















 



















 











 























 





 



 










 














 













 








 













 













 















 











 








 








 






 





 












 





 













 






 


   


  







 







 




 






 




 




 













 

   




 







 







 







 










 





 

















 


 


 













 

   


 


 



 

 

 
  typedef unsigned int _Wchart;
  typedef unsigned int _Wintt;

 

 
typedef unsigned int     _Sizet;

 
typedef signed char   __int8_t;
typedef unsigned char  __uint8_t;
typedef signed short int   __int16_t;
typedef unsigned short int  __uint16_t;
typedef signed int   __int32_t;
typedef unsigned int  __uint32_t;
   typedef signed long long int   __int64_t;
   typedef unsigned long long int  __uint64_t;
typedef signed int   __intptr_t;
typedef unsigned int  __uintptr_t;

 
typedef struct _Mbstatet
{  
    unsigned int _Wchar;   
    unsigned int _State;   

} _Mbstatet;

 

 
  typedef struct __va_list __Va_list;



 
typedef struct
{
    long long _Off;     
  _Mbstatet _Wstate;
} _Fpost;


 

 
  
   
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Malloc(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Stream(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Debug(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_StaticGuard(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Malloc(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Stream(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Debug(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_StaticGuard(void);


  typedef void *__iar_Rmtx;

  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Initdynamiclock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Dstdynamiclock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Lockdynamiclock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlockdynamiclock(__iar_Rmtx *);

  






 


 
  typedef signed char          int8_t;
  typedef unsigned char        uint8_t;

  typedef signed short int         int16_t;
  typedef unsigned short int       uint16_t;

  typedef signed int         int32_t;
  typedef unsigned int       uint32_t;

  typedef signed long long int         int64_t;
  typedef unsigned long long int       uint64_t;


 
typedef signed char      int_least8_t;
typedef unsigned char    uint_least8_t;

typedef signed short int     int_least16_t;
typedef unsigned short int   uint_least16_t;

typedef signed int     int_least32_t;
typedef unsigned int   uint_least32_t;

 
  typedef signed long long int   int_least64_t;
  typedef unsigned long long int uint_least64_t;



 
typedef signed int       int_fast8_t;
typedef unsigned int     uint_fast8_t;

typedef signed int      int_fast16_t;
typedef unsigned int    uint_fast16_t;

typedef signed int      int_fast32_t;
typedef unsigned int    uint_fast32_t;

  typedef signed long long int    int_fast64_t;
  typedef unsigned long long int  uint_fast64_t;

 
typedef signed long long int          intmax_t;
typedef unsigned long long int        uintmax_t;


 
typedef signed int          intptr_t;
typedef unsigned int        uintptr_t;

 
typedef int __data_intptr_t; typedef unsigned int __data_uintptr_t;

 






















 











 
















































 



 



 
    




 



 



 
  


 




 
typedef enum
{
 
  NonMaskableInt_IRQn         = -14,     
  MemoryManagement_IRQn       = -12,     
  BusFault_IRQn               = -11,     
  UsageFault_IRQn             = -10,     
  SVCall_IRQn                 = -5,      
  DebugMonitor_IRQn           = -4,      
  PendSV_IRQn                 = -2,      
  SysTick_IRQn                = -1,      
 
  WWDG_IRQn                   = 0,       
  PVD_IRQn                    = 1,       
  TAMP_STAMP_IRQn             = 2,       
  RTC_WKUP_IRQn               = 3,       
  FLASH_IRQn                  = 4,       
  RCC_IRQn                    = 5,       
  EXTI0_IRQn                  = 6,       
  EXTI1_IRQn                  = 7,       
  EXTI2_IRQn                  = 8,       
  EXTI3_IRQn                  = 9,       
  EXTI4_IRQn                  = 10,      
  DMA1_Stream0_IRQn           = 11,      
  DMA1_Stream1_IRQn           = 12,      
  DMA1_Stream2_IRQn           = 13,      
  DMA1_Stream3_IRQn           = 14,      
  DMA1_Stream4_IRQn           = 15,      
  DMA1_Stream5_IRQn           = 16,      
  DMA1_Stream6_IRQn           = 17,      
  ADC_IRQn                    = 18,      
  EXTI9_5_IRQn                = 23,      
  TIM1_BRK_TIM9_IRQn          = 24,      
  TIM1_UP_TIM10_IRQn          = 25,      
  TIM1_TRG_COM_TIM11_IRQn     = 26,      
  TIM1_CC_IRQn                = 27,      
  TIM2_IRQn                   = 28,      
  TIM3_IRQn                   = 29,      
  TIM4_IRQn                   = 30,      
  I2C1_EV_IRQn                = 31,      
  I2C1_ER_IRQn                = 32,      
  I2C2_EV_IRQn                = 33,      
  I2C2_ER_IRQn                = 34,      
  SPI1_IRQn                   = 35,      
  SPI2_IRQn                   = 36,      
  USART1_IRQn                 = 37,      
  USART2_IRQn                 = 38,      
  EXTI15_10_IRQn              = 40,      
  RTC_Alarm_IRQn              = 41,      
  OTG_FS_WKUP_IRQn            = 42,      
  DMA1_Stream7_IRQn           = 47,      
  SDIO_IRQn                   = 49,      
  TIM5_IRQn                   = 50,      
  SPI3_IRQn                   = 51,      
  DMA2_Stream0_IRQn           = 56,      
  DMA2_Stream1_IRQn           = 57,      
  DMA2_Stream2_IRQn           = 58,      
  DMA2_Stream3_IRQn           = 59,      
  DMA2_Stream4_IRQn           = 60,      
  OTG_FS_IRQn                 = 67,      
  DMA2_Stream5_IRQn           = 68,      
  DMA2_Stream6_IRQn           = 69,      
  DMA2_Stream7_IRQn           = 70,      
  USART6_IRQn                 = 71,      
  I2C3_EV_IRQn                = 72,      
  I2C3_ER_IRQn                = 73,      
  FPU_IRQn                    = 81,      
  SPI4_IRQn                   = 84,      
  SPI5_IRQn                   = 85       
} IRQn_Type;



 

 




 
















 

  #pragma system_include          
















 




 



 

 




 
















 

  #pragma system_include          


 
 
 




 


 




 
















 





 
 




 





















#pragma system_include






 

 














#pragma language=save
#pragma language=extended
_Pragma("inline=forced") __intrinsic uint16_t __iar_uint16_read(void const *ptr)
{
  return *(__packed uint16_t*)(ptr);
}
#pragma language=restore


#pragma language=save
#pragma language=extended
_Pragma("inline=forced") __intrinsic void __iar_uint16_write(void const *ptr, uint16_t val)
{
  *(__packed uint16_t*)(ptr) = val;;
}
#pragma language=restore

#pragma language=save
#pragma language=extended
_Pragma("inline=forced") __intrinsic uint32_t __iar_uint32_read(void const *ptr)
{
  return *(__packed uint32_t*)(ptr);
}
#pragma language=restore

#pragma language=save
#pragma language=extended
_Pragma("inline=forced") __intrinsic void __iar_uint32_write(void const *ptr, uint32_t val)
{
  *(__packed uint32_t*)(ptr) = val;;
}
#pragma language=restore

#pragma language=save
#pragma language=extended
__packed struct  __iar_u32 { uint32_t v; };
#pragma language=restore















 



  #pragma system_include



 


 


#pragma language=save
#pragma language=extended

__intrinsic __nounwind void    __iar_builtin_no_operation(void);

__intrinsic __nounwind void    __iar_builtin_disable_interrupt(void);
__intrinsic __nounwind void    __iar_builtin_enable_interrupt(void);

typedef unsigned int __istate_t;

__intrinsic __nounwind __istate_t __iar_builtin_get_interrupt_state(void);
__intrinsic __nounwind void __iar_builtin_set_interrupt_state(__istate_t);

 
__intrinsic __nounwind unsigned int __iar_builtin_get_PSR( void );
__intrinsic __nounwind unsigned int __iar_builtin_get_IPSR( void );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_get_MSP( void );
__intrinsic __nounwind void         __iar_builtin_set_MSP( unsigned int );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_get_PSP( void );
__intrinsic __nounwind void         __iar_builtin_set_PSP( unsigned int );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_get_PRIMASK( void );
__intrinsic __nounwind void         __iar_builtin_set_PRIMASK( unsigned int );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_get_CONTROL( void );
__intrinsic __nounwind void         __iar_builtin_set_CONTROL( unsigned int );

 
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_get_FAULTMASK( void );
__intrinsic __nounwind void         __iar_builtin_set_FAULTMASK(unsigned int);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_get_BASEPRI( void );
__intrinsic __nounwind void         __iar_builtin_set_BASEPRI( unsigned int );

 
__intrinsic __nounwind void __iar_builtin_disable_irq(void);
__intrinsic __nounwind void __iar_builtin_enable_irq(void);

__intrinsic __nounwind void __iar_builtin_disable_fiq(void);
__intrinsic __nounwind void __iar_builtin_enable_fiq(void);


 

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SWP( unsigned int, volatile unsigned int * );
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned char __iar_builtin_SWPB( unsigned char, volatile unsigned char * );

typedef unsigned int __ul;
typedef unsigned int __iar_builtin_uint;


 

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind void __iar_builtin_CDP (unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) opc1, unsigned __constrange(0,15) CRd, unsigned __constrange(0,15) CRn, unsigned __constrange(0,15) CRm, unsigned __constrange(0,8) opc2) ;
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind void __iar_builtin_CDP2(unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) opc1, unsigned __constrange(0,15) CRd, unsigned __constrange(0,15) CRn, unsigned __constrange(0,15) CRm, unsigned __constrange(0,8) opc2) ;

 
__intrinsic __nounwind void          __iar_builtin_MCR( unsigned __constrange(0,15) coproc, unsigned __constrange(0,8) opcode_1, __iar_builtin_uint src,
                                unsigned __constrange(0,15) CRn, unsigned __constrange(0,15) CRm, unsigned __constrange(0,8) opcode_2 )  ;
__intrinsic __nounwind unsigned int __iar_builtin_MRC( unsigned __constrange(0,15) coproc, unsigned __constrange(0,8) opcode_1,
                                unsigned __constrange(0,15) CRn, unsigned __constrange(0,15) CRm, unsigned __constrange(0,8) opcode_2 )  ;
__intrinsic __nounwind void          __iar_builtin_MCR2( unsigned __constrange(0,15) coproc, unsigned __constrange(0,8) opcode_1, __iar_builtin_uint src,
                                 unsigned __constrange(0,15) CRn, unsigned __constrange(0,15) CRm, unsigned __constrange(0,8) opcode_2 ) ;
__intrinsic __nounwind unsigned int __iar_builtin_MRC2( unsigned __constrange(0,15) coproc, unsigned __constrange(0,8) opcode_1,
                                 unsigned __constrange(0,15) CRn, unsigned __constrange(0,15) CRm, unsigned __constrange(0,8) opcode_2 ) ;

__intrinsic __nounwind void __iar_builtin_MCRR (unsigned __constrange(0,15) coproc, unsigned __constrange(0,8) opc1, unsigned long long src, unsigned __constrange(0,15) CRm) ;
__intrinsic __nounwind void __iar_builtin_MCRR2(unsigned __constrange(0,15) coproc, unsigned __constrange(0,8) opc1, unsigned long long src, unsigned __constrange(0,15) CRm) ;

__intrinsic __nounwind unsigned long long __iar_builtin_MRRC (unsigned __constrange(0,15) coproc, unsigned __constrange(0,8) opc1, unsigned __constrange(0,15) CRm) ;
__intrinsic __nounwind unsigned long long __iar_builtin_MRRC2(unsigned __constrange(0,15) coproc, unsigned __constrange(0,8) opc1, unsigned __constrange(0,15) CRm) ;

 
__intrinsic __nounwind void __iar_builtin_LDC  ( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint const *src) ;
__intrinsic __nounwind void __iar_builtin_LDCL ( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint const *src) ;
__intrinsic __nounwind void __iar_builtin_LDC2 ( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint const *src) ;
__intrinsic __nounwind void __iar_builtin_LDC2L( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint const *src) ;

 
__intrinsic __nounwind void __iar_builtin_STC  ( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint *dst) ;
__intrinsic __nounwind void __iar_builtin_STCL ( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint *dst) ;
__intrinsic __nounwind void __iar_builtin_STC2 ( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint *dst) ;
__intrinsic __nounwind void __iar_builtin_STC2L( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint *dst) ;

 
__intrinsic __nounwind void __iar_builtin_LDC_noidx( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint const *src,
                              unsigned __constrange(0,255) option);

__intrinsic __nounwind void __iar_builtin_LDCL_noidx( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint const *src,
                               unsigned __constrange(0,255) option);

__intrinsic __nounwind void __iar_builtin_LDC2_noidx( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint const *src,
                               unsigned __constrange(0,255) option);

__intrinsic __nounwind void __iar_builtin_LDC2L_noidx( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint const *src,
                                unsigned __constrange(0,255) option);

 
__intrinsic __nounwind void __iar_builtin_STC_noidx( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint *dst,
                              unsigned __constrange(0,255) option);

__intrinsic __nounwind void __iar_builtin_STCL_noidx( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint *dst,
                               unsigned __constrange(0,255) option);

__intrinsic __nounwind void __iar_builtin_STC2_noidx( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint *dst,
                               unsigned __constrange(0,255) option);

__intrinsic __nounwind void __iar_builtin_STC2L_noidx( unsigned __constrange(0,15) coproc, unsigned __constrange(0,15) CRn, volatile __iar_builtin_uint *dst,
                                unsigned __constrange(0,255) option);


 
__intrinsic __nounwind unsigned int       __iar_builtin_rsr(__spec_string const char * special_register)   ;
__intrinsic __nounwind unsigned long long __iar_builtin_rsr64(__spec_string const char * special_register) ;
__intrinsic __nounwind void*              __iar_builtin_rsrp(__spec_string const char * special_register)  ;

 
__intrinsic __nounwind void __iar_builtin_wsr(__spec_string const char * special_register, unsigned int value)         ;
__intrinsic __nounwind void __iar_builtin_wsr64(__spec_string const char * special_register, unsigned long long value) ;
__intrinsic __nounwind void __iar_builtin_wsrp(__spec_string const char * special_register, const void *value)         ;

 
__intrinsic __nounwind unsigned int __iar_builtin_get_APSR( void );
__intrinsic __nounwind void         __iar_builtin_set_APSR( unsigned int );

 
__intrinsic __nounwind unsigned int __iar_builtin_get_CPSR( void );
__intrinsic __nounwind void         __iar_builtin_set_CPSR( unsigned int );

 
__intrinsic __nounwind unsigned int __iar_builtin_get_FPSCR( void );
__intrinsic __nounwind void __iar_builtin_set_FPSCR( unsigned int );

 
 
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_CLZ(unsigned int) ;
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_ROR(unsigned int, unsigned int) ;
__intrinsic __nounwind unsigned int __iar_builtin_RRX(unsigned int);

 
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind signed int __iar_builtin_QADD( signed int, signed int );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind signed int __iar_builtin_QDADD( signed int, signed int );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind signed int __iar_builtin_QSUB( signed int, signed int );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind signed int __iar_builtin_QDSUB( signed int, signed int );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind signed int __iar_builtin_QDOUBLE( signed int );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int        __iar_builtin_QFlag( void );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int   __iar_builtin_acle_QFlag(void);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind void  __iar_builtin_set_QFlag(int);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind void  __iar_builtin_ignore_QFlag(void);

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int         __iar_builtin_QCFlag( void );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind void __iar_builtin_reset_QC_flag( void );

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind signed int __iar_builtin_SMUL( signed short, signed short );

 
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_REV( unsigned int );
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind signed int __iar_builtin_REVSH( short );

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_REV16( unsigned int );
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_RBIT( unsigned int );

__intrinsic __nounwind unsigned char  __iar_builtin_LDREXB( volatile unsigned char const * );
__intrinsic __nounwind unsigned short __iar_builtin_LDREXH( volatile unsigned short const * );
__intrinsic __nounwind unsigned int  __iar_builtin_LDREX ( volatile unsigned int const * );
__intrinsic __nounwind unsigned long long __iar_builtin_LDREXD( volatile unsigned long long const * );

__intrinsic __nounwind unsigned int  __iar_builtin_STREXB( unsigned char, volatile unsigned char * );
__intrinsic __nounwind unsigned int  __iar_builtin_STREXH( unsigned short, volatile unsigned short * );
__intrinsic __nounwind unsigned int  __iar_builtin_STREX ( unsigned int, volatile unsigned int * );
__intrinsic __nounwind unsigned int  __iar_builtin_STREXD( unsigned long long, volatile unsigned long long * );

__intrinsic __nounwind void __iar_builtin_CLREX( void );

__intrinsic __nounwind void __iar_builtin_SEV( void );
__intrinsic __nounwind void __iar_builtin_WFE( void );
__intrinsic __nounwind void __iar_builtin_WFI( void );
__intrinsic __nounwind void __iar_builtin_YIELD( void );

__intrinsic __nounwind void __iar_builtin_PLI( volatile void const * );
__intrinsic __nounwind void __iar_builtin_PLD( volatile void const * );

__intrinsic __nounwind void __iar_builtin_PLIx( volatile void const *, unsigned int __constrange(0,2), unsigned int __constrange(0,1));
__intrinsic __nounwind void __iar_builtin_PLDx( volatile void const *, unsigned int __constrange(0, 1), unsigned int __constrange(0, 2), unsigned int __constrange(0, 1));
__intrinsic __nounwind void __iar_builtin_PLDW( volatile void const * );

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind signed int   __iar_builtin_SSAT     (signed int val, unsigned int __constrange( 1, 32 ) sat );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_USAT     (signed int val, unsigned int __constrange( 0, 31 ) sat );

 
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SEL( unsigned int op1, unsigned int op2 );

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SADD8    (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SADD16   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SSUB8    (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SSUB16   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SADDSUBX (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SSUBADDX (unsigned int pair1, unsigned int pair2);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SHADD8   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SHADD16  (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SHSUB8   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SHSUB16  (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SHADDSUBX(unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SHSUBADDX(unsigned int pair1, unsigned int pair2);

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_QADD8    (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_QADD16   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_QSUB8    (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_QSUB16   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_QADDSUBX (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_QSUBADDX (unsigned int pair1, unsigned int pair2);

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UADD8    (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UADD16   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_USUB8    (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_USUB16   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UADDSUBX (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_USUBADDX (unsigned int pair1, unsigned int pair2);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UHADD8   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UHADD16  (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UHSUB8   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UHSUB16  (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UHADDSUBX(unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UHSUBADDX(unsigned int pair1, unsigned int pair2);

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UQADD8   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UQADD16  (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UQSUB8   (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UQSUB16  (unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UQADDSUBX(unsigned int pair1, unsigned int pair2);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UQSUBADDX(unsigned int pair1, unsigned int pair2);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_USAD8(unsigned int x, unsigned int y );
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_USADA8(unsigned int x, unsigned int y,
                                   unsigned int acc );

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SSAT16   (unsigned int pair,
                                      unsigned int __constrange( 1, 16 ) sat );
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_USAT16   (unsigned int pair,
                                      unsigned int __constrange( 0, 15 ) sat );

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMUAD (unsigned int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMUSD (unsigned int x, unsigned int y);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMUADX(unsigned int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMUSDX(unsigned int x, unsigned int y);

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLAD (unsigned int x, unsigned int y, int sum);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLSD (unsigned int x, unsigned int y, int sum);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLADX(unsigned int x, unsigned int y, int sum);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLSDX(unsigned int x, unsigned int y, int sum);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind long long __iar_builtin_SMLALD (unsigned int pair1,
                                 unsigned int pair2,
                                 long long acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind long long __iar_builtin_SMLALDX(unsigned int pair1,
                                 unsigned int pair2,
                                 long long acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind long long __iar_builtin_SMLSLD (unsigned int pair1,
                                 unsigned int pair2,
                                 long long acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind long long __iar_builtin_SMLSLDX(unsigned int pair1,
                                 unsigned int pair2,
                                 long long acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_PKHBT(unsigned int x,
                                  unsigned int y,
                                  unsigned __constrange(0,31) count);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_PKHTB(unsigned int x,
                                  unsigned int y,
                                  unsigned __constrange(0,32) count);

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLABB(unsigned int x, unsigned int y, int acc);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLABT(unsigned int x, unsigned int y, int acc);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLATB(unsigned int x, unsigned int y, int acc);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLATT(unsigned int x, unsigned int y, int acc);

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLAWB(int x, unsigned int y, int acc);
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMLAWT(int x, unsigned int y, int acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMMLA (int x, int y, int acc);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMMLAR(int x, int y, int acc);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMMLS (int x, int y, int acc);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMMLSR(int x, int y, int acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMMUL (int x, int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMMULR(int x, int y);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMULBB(unsigned int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMULBT(unsigned int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMULTB(unsigned int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMULTT(unsigned int x, unsigned int y);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMULWB(int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SMULWT(int x, unsigned int y);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SXTAB (int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind int __iar_builtin_SXTAH (int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UXTAB (unsigned int x, unsigned int y);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UXTAH (unsigned int x, unsigned int y);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned long long __iar_builtin_UMAAL(unsigned int x,
                                       unsigned int y,
                                       unsigned int a,
                                       unsigned int b);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind long long __iar_builtin_SMLALBB(unsigned int x,
                                 unsigned int y,
                                 long long acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind long long __iar_builtin_SMLALBT(unsigned int x,
                                 unsigned int y,
                                 long long acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind long long __iar_builtin_SMLALTB(unsigned int x,
                                 unsigned int y,
                                 long long acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind long long __iar_builtin_SMLALTT(unsigned int x,
                                 unsigned int y,
                                 long long acc);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UXTB16(unsigned int x);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UXTAB16(unsigned int acc, unsigned int x);

_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SXTB16(unsigned int x);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SXTAB16(unsigned int acc, unsigned int x);

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SASX(unsigned int, unsigned int) ;
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SSAX(unsigned int, unsigned int) ;
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SHASX(unsigned int, unsigned int) ;
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_SHSAX(unsigned int, unsigned int) ;
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_QASX(unsigned int, unsigned int) ;
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_QSAX(unsigned int, unsigned int) ;

_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UASX(unsigned int, unsigned int) ;
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_USAX(unsigned int, unsigned int) ;
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UHASX(unsigned int, unsigned int) ;
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UHSAX(unsigned int, unsigned int) ;
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UQASX(unsigned int, unsigned int) ;
_Pragma("function_effects = hidden_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_UQSAX(unsigned int, unsigned int) ;

 
__intrinsic __nounwind void __iar_builtin_DMB(void);
__intrinsic __nounwind void __iar_builtin_DSB(void);
__intrinsic __nounwind void __iar_builtin_ISB(void);
__intrinsic __nounwind void __iar_builtin_DMBx(unsigned int __constrange(1, 15)) ;
__intrinsic __nounwind void __iar_builtin_DSBx(unsigned int __constrange(1, 15)) ;
__intrinsic __nounwind void __iar_builtin_ISBx(unsigned int __constrange(1, 15)) ;

 
__intrinsic __nounwind unsigned int __iar_builtin_TT(unsigned int);
__intrinsic __nounwind unsigned int __iar_builtin_TTT(unsigned int);
__intrinsic __nounwind unsigned int __iar_builtin_TTA(unsigned int);
__intrinsic __nounwind unsigned int __iar_builtin_TTAT(unsigned int);

__intrinsic __nounwind unsigned int __get_LR(void);
__intrinsic __nounwind void __set_LR(unsigned int);

__intrinsic __nounwind unsigned int __get_SP(void);
__intrinsic __nounwind void __set_SP(unsigned int);

 
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VSQRT_F32(float x);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VSQRT_F64(double x);

 
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VFMA_F32(float x, float y, float z);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VFMS_F32(float x, float y, float z);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VFNMA_F32(float x, float y, float z);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VFNMS_F32(float x, float y, float z);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VFMA_F64(double x, double y, double z);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VFMS_F64(double x, double y, double z);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VFNMA_F64(double x, double y, double z);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VFNMS_F64(double x, double y, double z);

 
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_CRC32B(unsigned int crc, unsigned char data);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_CRC32H(unsigned int crc, unsigned short data);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_CRC32W(unsigned int crc, unsigned int data);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_CRC32CB(unsigned int crc, unsigned char data);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_CRC32CH(unsigned int crc, unsigned short data);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind unsigned int __iar_builtin_CRC32CW(unsigned int crc, unsigned int data);

 
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VMAXNM_F32(float a, float b);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VMINNM_F32(float a, float b);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VMAXNM_F64(double a, double b);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VMINNM_F64(double a, double b);

 
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VRINTA_F32(float a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VRINTM_F32(float a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VRINTN_F32(float a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VRINTP_F32(float a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VRINTX_F32(float a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VRINTR_F32(float a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind float __iar_builtin_VRINTZ_F32(float a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VRINTA_F64(double a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VRINTM_F64(double a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VRINTN_F64(double a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VRINTP_F64(double a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VRINTX_F64(double a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VRINTR_F64(double a);
_Pragma("function_effects = no_state, always_returns") __intrinsic __nounwind double __iar_builtin_VRINTZ_F64(double a);

#pragma language=restore









    
  
    
  

  
    
    







  _Pragma("inline=forced") __intrinsic int16_t __REVSH(int16_t val)
  {
    return (int16_t) __iar_builtin_REVSH(val);
  }












  _Pragma("inline=forced") __intrinsic uint8_t __LDRBT(volatile uint8_t *addr)
  {
    uint32_t res;
    __asm("LDRBT %0, [%1]" : "=r" (res) : "r" (addr) : "memory");
    return ((uint8_t)res);
  }

  _Pragma("inline=forced") __intrinsic uint16_t __LDRHT(volatile uint16_t *addr)
  {
    uint32_t res;
    __asm("LDRHT %0, [%1]" : "=r" (res) : "r" (addr) : "memory");
    return ((uint16_t)res);
  }

  _Pragma("inline=forced") __intrinsic uint32_t __LDRT(volatile uint32_t *addr)
  {
    uint32_t res;
    __asm("LDRT %0, [%1]" : "=r" (res) : "r" (addr) : "memory");
    return res;
  }

  _Pragma("inline=forced") __intrinsic void __STRBT(uint8_t value, volatile uint8_t *addr)
  {
    __asm("STRBT %1, [%0]" : : "r" (addr), "r" ((uint32_t)value) : "memory");
  }

  _Pragma("inline=forced") __intrinsic void __STRHT(uint16_t value, volatile uint16_t *addr)
  {
    __asm("STRHT %1, [%0]" : : "r" (addr), "r" ((uint32_t)value) : "memory");
  }

  _Pragma("inline=forced") __intrinsic void __STRT(uint32_t value, volatile uint32_t *addr)
  {
    __asm("STRT %1, [%0]" : : "r" (addr), "r" (value) : "memory");
  }




#pragma diag_default=Pe940
#pragma diag_default=Pe177


  


 










 

 






 

 

 













 



 






 



 
typedef union
{
  struct
  {
    uint32_t _reserved0:16;               
    uint32_t GE:4;                        
    uint32_t _reserved1:7;                
    uint32_t Q:1;                         
    uint32_t V:1;                         
    uint32_t C:1;                         
    uint32_t Z:1;                         
    uint32_t N:1;                         
  } b;                                    
  uint32_t w;                             
} APSR_Type;

 









 
typedef union
{
  struct
  {
    uint32_t ISR:9;                       
    uint32_t _reserved0:23;               
  } b;                                    
  uint32_t w;                             
} IPSR_Type;

 




 
typedef union
{
  struct
  {
    uint32_t ISR:9;                       
    uint32_t _reserved0:1;                
    uint32_t ICI_IT_1:6;                  
    uint32_t GE:4;                        
    uint32_t _reserved1:4;                
    uint32_t T:1;                         
    uint32_t ICI_IT_2:2;                  
    uint32_t Q:1;                         
    uint32_t V:1;                         
    uint32_t C:1;                         
    uint32_t Z:1;                         
    uint32_t N:1;                         
  } b;                                    
  uint32_t w;                             
} xPSR_Type;

 













 
typedef union
{
  struct
  {
    uint32_t nPRIV:1;                     
    uint32_t SPSEL:1;                     
    uint32_t FPCA:1;                      
    uint32_t _reserved0:29;               
  } b;                                    
  uint32_t w;                             
} CONTROL_Type;

 



 







 



 
typedef struct
{
  volatile uint32_t ISER[8U];                
        uint32_t RESERVED0[24U];
  volatile uint32_t ICER[8U];                
        uint32_t RSERVED1[24U];
  volatile uint32_t ISPR[8U];                
        uint32_t RESERVED2[24U];
  volatile uint32_t ICPR[8U];                
        uint32_t RESERVED3[24U];
  volatile uint32_t IABR[8U];                
        uint32_t RESERVED4[56U];
  volatile uint8_t  IP[240U];                
        uint32_t RESERVED5[644U];
  volatile  uint32_t STIR;                    
}  NVIC_Type;

 

 







 



 
typedef struct
{
  volatile const  uint32_t CPUID;                   
  volatile uint32_t ICSR;                    
  volatile uint32_t VTOR;                    
  volatile uint32_t AIRCR;                   
  volatile uint32_t SCR;                     
  volatile uint32_t CCR;                     
  volatile uint8_t  SHP[12U];                
  volatile uint32_t SHCSR;                   
  volatile uint32_t CFSR;                    
  volatile uint32_t HFSR;                    
  volatile uint32_t DFSR;                    
  volatile uint32_t MMFAR;                   
  volatile uint32_t BFAR;                    
  volatile uint32_t AFSR;                    
  volatile const  uint32_t PFR[2U];                 
  volatile const  uint32_t DFR;                     
  volatile const  uint32_t ADR;                     
  volatile const  uint32_t MMFR[4U];                
  volatile const  uint32_t ISAR[5U];                
        uint32_t RESERVED0[5U];
  volatile uint32_t CPACR;                   
} SCB_Type;

 





 










 

 







 



 






 














 



 






 







 






 



 





 







 



 
typedef struct
{
        uint32_t RESERVED0[1U];
  volatile const  uint32_t ICTR;                    
  volatile uint32_t ACTLR;                   
} SCnSCB_Type;

 

 





 







 



 
typedef struct
{
  volatile uint32_t CTRL;                    
  volatile uint32_t LOAD;                    
  volatile uint32_t VAL;                     
  volatile const  uint32_t CALIB;                   
} SysTick_Type;

 




 

 

 



 







 



 
typedef struct
{
  volatile  union
  {
    volatile  uint8_t    u8;                  
    volatile  uint16_t   u16;                 
    volatile  uint32_t   u32;                 
  }  PORT [32U];                          
        uint32_t RESERVED0[864U];
  volatile uint32_t TER;                     
        uint32_t RESERVED1[15U];
  volatile uint32_t TPR;                     
        uint32_t RESERVED2[15U];
  volatile uint32_t TCR;                     
        uint32_t RESERVED3[29U];
  volatile  uint32_t IWR;                     
  volatile const  uint32_t IRR;                     
  volatile uint32_t IMCR;                    
        uint32_t RESERVED4[43U];
  volatile  uint32_t LAR;                     
  volatile const  uint32_t LSR;                     
        uint32_t RESERVED5[6U];
  volatile const  uint32_t PID4;                    
  volatile const  uint32_t PID5;                    
  volatile const  uint32_t PID6;                    
  volatile const  uint32_t PID7;                    
  volatile const  uint32_t PID0;                    
  volatile const  uint32_t PID1;                    
  volatile const  uint32_t PID2;                    
  volatile const  uint32_t PID3;                    
  volatile const  uint32_t CID0;                    
  volatile const  uint32_t CID1;                    
  volatile const  uint32_t CID2;                    
  volatile const  uint32_t CID3;                    
} ITM_Type;

 

 









 

 

 

 



   







 



 
typedef struct
{
  volatile uint32_t CTRL;                    
  volatile uint32_t CYCCNT;                  
  volatile uint32_t CPICNT;                  
  volatile uint32_t EXCCNT;                  
  volatile uint32_t SLEEPCNT;                
  volatile uint32_t LSUCNT;                  
  volatile uint32_t FOLDCNT;                 
  volatile const  uint32_t PCSR;                    
  volatile uint32_t COMP0;                   
  volatile uint32_t MASK0;                   
  volatile uint32_t FUNCTION0;               
        uint32_t RESERVED0[1U];
  volatile uint32_t COMP1;                   
  volatile uint32_t MASK1;                   
  volatile uint32_t FUNCTION1;               
        uint32_t RESERVED1[1U];
  volatile uint32_t COMP2;                   
  volatile uint32_t MASK2;                   
  volatile uint32_t FUNCTION2;               
        uint32_t RESERVED2[1U];
  volatile uint32_t COMP3;                   
  volatile uint32_t MASK3;                   
  volatile uint32_t FUNCTION3;               
} DWT_Type;

 


















 

 

 

 

 

 

 









   







 



 
typedef struct
{
  volatile uint32_t SSPSR;                   
  volatile uint32_t CSPSR;                   
        uint32_t RESERVED0[2U];
  volatile uint32_t ACPR;                    
        uint32_t RESERVED1[55U];
  volatile uint32_t SPPR;                    
        uint32_t RESERVED2[131U];
  volatile const  uint32_t FFSR;                    
  volatile uint32_t FFCR;                    
  volatile const  uint32_t FSCR;                    
        uint32_t RESERVED3[759U];
  volatile const  uint32_t TRIGGER;                 
  volatile const  uint32_t FIFO0;                   
  volatile const  uint32_t ITATBCTR2;               
        uint32_t RESERVED4[1U];
  volatile const  uint32_t ITATBCTR0;               
  volatile const  uint32_t FIFO1;                   
  volatile uint32_t ITCTRL;                  
        uint32_t RESERVED5[39U];
  volatile uint32_t CLAIMSET;                
  volatile uint32_t CLAIMCLR;                
        uint32_t RESERVED7[8U];
  volatile const  uint32_t DEVID;                   
  volatile const  uint32_t DEVTYPE;                 
} TPI_Type;

 

 

 




 


 

 







 

 







 

 

 






 


   







 



 
typedef struct
{
  volatile const  uint32_t TYPE;                    
  volatile uint32_t CTRL;                    
  volatile uint32_t RNR;                     
  volatile uint32_t RBAR;                    
  volatile uint32_t RASR;                    
  volatile uint32_t RBAR_A1;                 
  volatile uint32_t RASR_A1;                 
  volatile uint32_t RBAR_A2;                 
  volatile uint32_t RASR_A2;                 
  volatile uint32_t RBAR_A3;                 
  volatile uint32_t RASR_A3;                 
} MPU_Type;


 



 



 

 



 










 







 



 
typedef struct
{
        uint32_t RESERVED0[1U];
  volatile uint32_t FPCCR;                   
  volatile uint32_t FPCAR;                   
  volatile uint32_t FPDSCR;                  
  volatile const  uint32_t MVFR0;                   
  volatile const  uint32_t MVFR1;                   
} FPU_Type;

 









 

 




 








 




 







 



 
typedef struct
{
  volatile uint32_t DHCSR;                   
  volatile  uint32_t DCRSR;                   
  volatile uint32_t DCRDR;                   
  volatile uint32_t DEMCR;                   
} CoreDebug_Type;

 












 


 













 







 






 






 

 







 

 




 










 


 



 





 














 
static inline void __NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);              

  reg_value  =  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR;                                                    
  reg_value &= ~((uint32_t)((0xFFFFUL << 16U) | (7UL << 8U)));  
  reg_value  =  (reg_value                                   |
                ((uint32_t)0x5FAUL << 16U) |
                (PriorityGroupTmp << 8U)                      );               
  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR =  reg_value;
}






 
static inline uint32_t __NVIC_GetPriorityGrouping(void)
{
  return ((uint32_t)((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR & (7UL << 8U)) >> 8U));
}







 
static inline void __NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISER[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
  }
}









 
static inline uint32_t __NVIC_GetEnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISER[(((uint32_t)(int32_t)IRQn) >> 5UL)] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}







 
static inline void __NVIC_DisableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ICER[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
    __iar_builtin_DSB();
    __iar_builtin_ISB();
  }
}









 
static inline uint32_t __NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISPR[(((uint32_t)(int32_t)IRQn) >> 5UL)] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}







 
static inline void __NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISPR[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
  }
}







 
static inline void __NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ICPR[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
  }
}









 
static inline uint32_t __NVIC_GetActive(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IABR[(((uint32_t)(int32_t)IRQn) >> 5UL)] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}










 
static inline void __NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[((uint32_t)(int32_t)IRQn)]               = (uint8_t)((priority << (8U - 4U)) & (uint32_t)0xFFUL);
  }
  else
  {
    ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[(((uint32_t)(int32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - 4U)) & (uint32_t)0xFFUL);
  }
}










 
static inline uint32_t __NVIC_GetPriority(IRQn_Type IRQn)
{

  if ((int32_t)(IRQn) >= 0)
  {
    return(((uint32_t)((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[((uint32_t)(int32_t)IRQn)]               >> (8U - 4U)));
  }
  else
  {
    return(((uint32_t)((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[(((uint32_t)(int32_t)IRQn) & 0xFUL)-4UL] >> (8U - 4U)));
  }
}












 
static inline uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);    
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(4U)) ? (uint32_t)(4U) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits     = ((PriorityGroupTmp + (uint32_t)(4U)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(4U));

  return (
           ((PreemptPriority & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
           ((SubPriority     & (uint32_t)((1UL << (SubPriorityBits    )) - 1UL)))
         );
}












 
static inline void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* const pPreemptPriority, uint32_t* const pSubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);    
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(4U)) ? (uint32_t)(4U) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits     = ((PriorityGroupTmp + (uint32_t)(4U)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(4U));

  *pPreemptPriority = (Priority >> SubPriorityBits) & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL);
  *pSubPriority     = (Priority                   ) & (uint32_t)((1UL << (SubPriorityBits    )) - 1UL);
}










 
static inline void __NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)
{
  uint32_t *vectors = (uint32_t *)((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->VTOR;
  vectors[(int32_t)IRQn + 16] = vector;
}









 
static inline uint32_t __NVIC_GetVector(IRQn_Type IRQn)
{
  uint32_t *vectors = (uint32_t *)((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->VTOR;
  return vectors[(int32_t)IRQn + 16];
}





 
static inline void __NVIC_SystemReset(void)
{
  __iar_builtin_DSB();                                                          
 
  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR  = (uint32_t)((0x5FAUL << 16U)    |
                           (((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR & (7UL << 8U)) |
                            (1UL << 2U)    );          
  __iar_builtin_DSB();                                                           

  for(;;)                                                            
  {
    __iar_builtin_no_operation();
  }
}

 

 







 
















 
 
  #pragma system_include          
 







 












                          




 
typedef struct _ARM_MPU_Region_t {
  uint32_t RBAR; 
  uint32_t RASR; 
} ARM_MPU_Region_t;
    


 
static inline void ARM_MPU_Enable(uint32_t MPU_Control)
{
  __iar_builtin_DSB();
  __iar_builtin_ISB();
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->CTRL = MPU_Control | (1UL );
  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHCSR |= (1UL << 16U);
}


 
static inline void ARM_MPU_Disable(void)
{
  __iar_builtin_DSB();
  __iar_builtin_ISB();
  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHCSR &= ~(1UL << 16U);
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->CTRL  &= ~(1UL );
}



 
static inline void ARM_MPU_ClrRegion(uint32_t rnr)
{
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RNR = rnr;
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RASR = 0U;
}




    
static inline void ARM_MPU_SetRegion(uint32_t rbar, uint32_t rasr)
{
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RBAR = rbar;
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RASR = rasr;
}





    
static inline void ARM_MPU_SetRegionEx(uint32_t rnr, uint32_t rbar, uint32_t rasr)
{
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RNR = rnr;
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RBAR = rbar;
  ((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RASR = rasr;
}





 
static inline void orderedCpy(volatile uint32_t* dst, const uint32_t* restrict src, uint32_t len)
{
  uint32_t i;
  for (i = 0U; i < len; ++i) 
  {
    dst[i] = src[i];
  }
}




 
static inline void ARM_MPU_Load(ARM_MPU_Region_t const* table, uint32_t cnt) 
{
  const uint32_t rowWordSize = sizeof(ARM_MPU_Region_t)/4U;
  while (cnt > 4U) {
    orderedCpy(&(((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RBAR), &(table->RBAR), 4U*rowWordSize);
    table += 4U;
    cnt -= 4U;
  }
  orderedCpy(&(((MPU_Type *) ((0xE000E000UL) + 0x0D90UL) )->RBAR), &(table->RBAR), cnt*rowWordSize);
}




 





 








 
static inline uint32_t SCB_GetFPUType(void)
{
  uint32_t mvfr0;

  mvfr0 = ((FPU_Type *) ((0xE000E000UL) + 0x0F30UL) )->MVFR0;
  if      ((mvfr0 & ((0xFUL << 4U) | (0xFUL << 8U))) == 0x020U)
  {
    return 1U;            
  }
  else
  {
    return 0U;            
  }
}


 



 





 












 
static inline uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > (0xFFFFFFUL ))
  {
    return (1UL);                                                    
  }

  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->LOAD  = (uint32_t)(ticks - 1UL);                          
  __NVIC_SetPriority (SysTick_IRQn, (1UL << 4U) - 1UL);  
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->VAL   = 0UL;                                              
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL  = (1UL << 2U) |
                   (1UL << 1U)   |
                   (1UL );                          
  return (0UL);                                                      
}


 



 





 

extern volatile int32_t ITM_RxBuffer;                               









 
static inline uint32_t ITM_SendChar (uint32_t ch)
{
  if (((((ITM_Type *) (0xE0000000UL) )->TCR & (1UL )) != 0UL) &&       
      ((((ITM_Type *) (0xE0000000UL) )->TER & 1UL               ) != 0UL)   )      
  {
    while (((ITM_Type *) (0xE0000000UL) )->PORT[0U].u32 == 0UL)
    {
      __iar_builtin_no_operation();
    }
    ((ITM_Type *) (0xE0000000UL) )->PORT[0U].u8 = (uint8_t)ch;
  }
  return (ch);
}







 
static inline int32_t ITM_ReceiveChar (void)
{
  int32_t ch = -1;                            

  if (ITM_RxBuffer != ((int32_t)0x5AA55AA5U))
  {
    ch = ITM_RxBuffer;
    ITM_RxBuffer = ((int32_t)0x5AA55AA5U);        
  }

  return (ch);
}







 
static inline int32_t ITM_CheckChar (void)
{

  if (ITM_RxBuffer == ((int32_t)0x5AA55AA5U))
  {
    return (0);                               
  }
  else
  {
    return (1);                               
  }
}

 







































  



 



   
  


 




 



 




 
  






 
extern uint32_t SystemCoreClock;           

extern const uint8_t  AHBPrescTable[16];     
extern const uint8_t  APBPrescTable[8];      



 



 



 



 



 



 
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);


 





 
  


   
 



    



 

typedef struct
{
  volatile uint32_t SR;      
  volatile uint32_t CR1;     
  volatile uint32_t CR2;     
  volatile uint32_t SMPR1;   
  volatile uint32_t SMPR2;   
  volatile uint32_t JOFR1;   
  volatile uint32_t JOFR2;   
  volatile uint32_t JOFR3;   
  volatile uint32_t JOFR4;   
  volatile uint32_t HTR;     
  volatile uint32_t LTR;     
  volatile uint32_t SQR1;    
  volatile uint32_t SQR2;    
  volatile uint32_t SQR3;    
  volatile uint32_t JSQR;    
  volatile uint32_t JDR1;    
  volatile uint32_t JDR2;    
  volatile uint32_t JDR3;    
  volatile uint32_t JDR4;    
  volatile uint32_t DR;      
} ADC_TypeDef;

typedef struct
{
  volatile uint32_t CSR;     
  volatile uint32_t CCR;     
  volatile uint32_t CDR;    
 
} ADC_Common_TypeDef;



 

typedef struct
{
  volatile uint32_t DR;          
  volatile uint8_t  IDR;         
  uint8_t       RESERVED0;   
  uint16_t      RESERVED1;   
  volatile uint32_t CR;          
} CRC_TypeDef;



 

typedef struct
{
  volatile uint32_t IDCODE;   
  volatile uint32_t CR;       
  volatile uint32_t APB1FZ;   
  volatile uint32_t APB2FZ;   
}DBGMCU_TypeDef;




 

typedef struct
{
  volatile uint32_t CR;      
  volatile uint32_t NDTR;    
  volatile uint32_t PAR;     
  volatile uint32_t M0AR;    
  volatile uint32_t M1AR;    
  volatile uint32_t FCR;     
} DMA_Stream_TypeDef;

typedef struct
{
  volatile uint32_t LISR;    
  volatile uint32_t HISR;    
  volatile uint32_t LIFCR;   
  volatile uint32_t HIFCR;   
} DMA_TypeDef;



 

typedef struct
{
  volatile uint32_t IMR;     
  volatile uint32_t EMR;     
  volatile uint32_t RTSR;    
  volatile uint32_t FTSR;    
  volatile uint32_t SWIER;   
  volatile uint32_t PR;      
} EXTI_TypeDef;



 

typedef struct
{
  volatile uint32_t ACR;       
  volatile uint32_t KEYR;      
  volatile uint32_t OPTKEYR;   
  volatile uint32_t SR;        
  volatile uint32_t CR;        
  volatile uint32_t OPTCR;     
  volatile uint32_t OPTCR1;    
} FLASH_TypeDef;



 

typedef struct
{
  volatile uint32_t MODER;     
  volatile uint32_t OTYPER;    
  volatile uint32_t OSPEEDR;   
  volatile uint32_t PUPDR;     
  volatile uint32_t IDR;       
  volatile uint32_t ODR;       
  volatile uint32_t BSRR;      
  volatile uint32_t LCKR;      
  volatile uint32_t AFR[2];    
} GPIO_TypeDef;



 

typedef struct
{
  volatile uint32_t MEMRMP;        
  volatile uint32_t PMC;           
  volatile uint32_t EXTICR[4];     
  uint32_t      RESERVED[2];   
  volatile uint32_t CMPCR;         
} SYSCFG_TypeDef;



 

typedef struct
{
  volatile uint32_t CR1;         
  volatile uint32_t CR2;         
  volatile uint32_t OAR1;        
  volatile uint32_t OAR2;        
  volatile uint32_t DR;          
  volatile uint32_t SR1;         
  volatile uint32_t SR2;         
  volatile uint32_t CCR;         
  volatile uint32_t TRISE;       
  volatile uint32_t FLTR;        
} I2C_TypeDef;



 

typedef struct
{
  volatile uint32_t KR;    
  volatile uint32_t PR;    
  volatile uint32_t RLR;   
  volatile uint32_t SR;    
} IWDG_TypeDef;




 

typedef struct
{
  volatile uint32_t CR;    
  volatile uint32_t CSR;   
} PWR_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;             
  volatile uint32_t PLLCFGR;        
  volatile uint32_t CFGR;           
  volatile uint32_t CIR;            
  volatile uint32_t AHB1RSTR;       
  volatile uint32_t AHB2RSTR;       
  volatile uint32_t AHB3RSTR;       
  uint32_t      RESERVED0;      
  volatile uint32_t APB1RSTR;       
  volatile uint32_t APB2RSTR;       
  uint32_t      RESERVED1[2];   
  volatile uint32_t AHB1ENR;        
  volatile uint32_t AHB2ENR;        
  volatile uint32_t AHB3ENR;        
  uint32_t      RESERVED2;      
  volatile uint32_t APB1ENR;        
  volatile uint32_t APB2ENR;        
  uint32_t      RESERVED3[2];   
  volatile uint32_t AHB1LPENR;      
  volatile uint32_t AHB2LPENR;      
  volatile uint32_t AHB3LPENR;      
  uint32_t      RESERVED4;      
  volatile uint32_t APB1LPENR;      
  volatile uint32_t APB2LPENR;      
  uint32_t      RESERVED5[2];   
  volatile uint32_t BDCR;           
  volatile uint32_t CSR;            
  uint32_t      RESERVED6[2];   
  volatile uint32_t SSCGR;          
  volatile uint32_t PLLI2SCFGR;     
  uint32_t      RESERVED7[1];   
  volatile uint32_t DCKCFGR;        
} RCC_TypeDef;



 

typedef struct
{
  volatile uint32_t TR;       
  volatile uint32_t DR;       
  volatile uint32_t CR;       
  volatile uint32_t ISR;      
  volatile uint32_t PRER;     
  volatile uint32_t WUTR;     
  volatile uint32_t CALIBR;   
  volatile uint32_t ALRMAR;   
  volatile uint32_t ALRMBR;   
  volatile uint32_t WPR;      
  volatile uint32_t SSR;      
  volatile uint32_t SHIFTR;   
  volatile uint32_t TSTR;     
  volatile uint32_t TSDR;     
  volatile uint32_t TSSSR;    
  volatile uint32_t CALR;     
  volatile uint32_t TAFCR;    
  volatile uint32_t ALRMASSR; 
  volatile uint32_t ALRMBSSR; 
  uint32_t RESERVED7;     
  volatile uint32_t BKP0R;    
  volatile uint32_t BKP1R;    
  volatile uint32_t BKP2R;    
  volatile uint32_t BKP3R;    
  volatile uint32_t BKP4R;    
  volatile uint32_t BKP5R;    
  volatile uint32_t BKP6R;    
  volatile uint32_t BKP7R;    
  volatile uint32_t BKP8R;    
  volatile uint32_t BKP9R;    
  volatile uint32_t BKP10R;   
  volatile uint32_t BKP11R;   
  volatile uint32_t BKP12R;   
  volatile uint32_t BKP13R;   
  volatile uint32_t BKP14R;   
  volatile uint32_t BKP15R;   
  volatile uint32_t BKP16R;   
  volatile uint32_t BKP17R;   
  volatile uint32_t BKP18R;   
  volatile uint32_t BKP19R;   
} RTC_TypeDef;



 

typedef struct
{
  volatile uint32_t POWER;                  
  volatile uint32_t CLKCR;                  
  volatile uint32_t ARG;                    
  volatile uint32_t CMD;                    
  volatile const uint32_t  RESPCMD;         
  volatile const uint32_t  RESP1;           
  volatile const uint32_t  RESP2;           
  volatile const uint32_t  RESP3;           
  volatile const uint32_t  RESP4;           
  volatile uint32_t DTIMER;                 
  volatile uint32_t DLEN;                   
  volatile uint32_t DCTRL;                  
  volatile const uint32_t  DCOUNT;          
  volatile const uint32_t  STA;             
  volatile uint32_t ICR;                    
  volatile uint32_t MASK;                   
  uint32_t      RESERVED0[2];           
  volatile const uint32_t  FIFOCNT;         
  uint32_t      RESERVED1[13];          
  volatile uint32_t FIFO;                   
} SDIO_TypeDef;



 

typedef struct
{
  volatile uint32_t CR1;         
  volatile uint32_t CR2;         
  volatile uint32_t SR;          
  volatile uint32_t DR;          
  volatile uint32_t CRCPR;       
  volatile uint32_t RXCRCR;      
  volatile uint32_t TXCRCR;      
  volatile uint32_t I2SCFGR;     
  volatile uint32_t I2SPR;       
} SPI_TypeDef;




 

typedef struct
{
  volatile uint32_t CR1;          
  volatile uint32_t CR2;          
  volatile uint32_t SMCR;         
  volatile uint32_t DIER;         
  volatile uint32_t SR;           
  volatile uint32_t EGR;          
  volatile uint32_t CCMR1;        
  volatile uint32_t CCMR2;        
  volatile uint32_t CCER;         
  volatile uint32_t CNT;          
  volatile uint32_t PSC;          
  volatile uint32_t ARR;          
  volatile uint32_t RCR;          
  volatile uint32_t CCR1;         
  volatile uint32_t CCR2;         
  volatile uint32_t CCR3;         
  volatile uint32_t CCR4;         
  volatile uint32_t BDTR;         
  volatile uint32_t DCR;          
  volatile uint32_t DMAR;         
  volatile uint32_t OR;           
} TIM_TypeDef;



 
 
typedef struct
{
  volatile uint32_t SR;          
  volatile uint32_t DR;          
  volatile uint32_t BRR;         
  volatile uint32_t CR1;         
  volatile uint32_t CR2;         
  volatile uint32_t CR3;         
  volatile uint32_t GTPR;        
} USART_TypeDef;



 

typedef struct
{
  volatile uint32_t CR;    
  volatile uint32_t CFR;   
  volatile uint32_t SR;    
} WWDG_TypeDef;


 
typedef struct
{
  volatile uint32_t GOTGCTL;               
  volatile uint32_t GOTGINT;               
  volatile uint32_t GAHBCFG;               
  volatile uint32_t GUSBCFG;               
  volatile uint32_t GRSTCTL;               
  volatile uint32_t GINTSTS;               
  volatile uint32_t GINTMSK;               
  volatile uint32_t GRXSTSR;               
  volatile uint32_t GRXSTSP;               
  volatile uint32_t GRXFSIZ;               
  volatile uint32_t DIEPTXF0_HNPTXFSIZ;    
  volatile uint32_t HNPTXSTS;              
  uint32_t Reserved30[2];              
  volatile uint32_t GCCFG;                 
  volatile uint32_t CID;                   
  uint32_t  Reserved40[48];            
  volatile uint32_t HPTXFSIZ;              
  volatile uint32_t DIEPTXF[0x0F];         
} USB_OTG_GlobalTypeDef;



 
typedef struct 
{
  volatile uint32_t DCFG;             
  volatile uint32_t DCTL;             
  volatile uint32_t DSTS;             
  uint32_t Reserved0C;            
  volatile uint32_t DIEPMSK;          
  volatile uint32_t DOEPMSK;          
  volatile uint32_t DAINT;            
  volatile uint32_t DAINTMSK;         
  uint32_t  Reserved20;           
  uint32_t Reserved9;             
  volatile uint32_t DVBUSDIS;         
  volatile uint32_t DVBUSPULSE;       
  volatile uint32_t DTHRCTL;          
  volatile uint32_t DIEPEMPMSK;       
  volatile uint32_t DEACHINT;         
  volatile uint32_t DEACHMSK;         
  uint32_t Reserved40;            
  volatile uint32_t DINEP1MSK;        
  uint32_t  Reserved44[15];       
  volatile uint32_t DOUTEP1MSK;       
} USB_OTG_DeviceTypeDef;



 
typedef struct 
{
  volatile uint32_t DIEPCTL;            
  uint32_t Reserved04;              
  volatile uint32_t DIEPINT;            
  uint32_t Reserved0C;              
  volatile uint32_t DIEPTSIZ;           
  volatile uint32_t DIEPDMA;            
  volatile uint32_t DTXFSTS;            
  uint32_t Reserved18;              
} USB_OTG_INEndpointTypeDef;



 
typedef struct 
{
  volatile uint32_t DOEPCTL;        
  uint32_t Reserved04;          
  volatile uint32_t DOEPINT;        
  uint32_t Reserved0C;          
  volatile uint32_t DOEPTSIZ;       
  volatile uint32_t DOEPDMA;        
  uint32_t Reserved18[2];       
} USB_OTG_OUTEndpointTypeDef;



 
typedef struct 
{
  volatile uint32_t HCFG;              
  volatile uint32_t HFIR;              
  volatile uint32_t HFNUM;             
  uint32_t Reserved40C;            
  volatile uint32_t HPTXSTS;           
  volatile uint32_t HAINT;             
  volatile uint32_t HAINTMSK;          
} USB_OTG_HostTypeDef;



 
typedef struct
{
  volatile uint32_t HCCHAR;            
  volatile uint32_t HCSPLT;            
  volatile uint32_t HCINT;             
  volatile uint32_t HCINTMSK;          
  volatile uint32_t HCTSIZ;            
  volatile uint32_t HCDMA;             
  uint32_t Reserved[2];            
} USB_OTG_HostChannelTypeDef;



 



 

 

 

 

 
 

 


 
 




 



   
 



 



 
  
  

 
    
 
 
 

 
 
 
 
 

 

 
  
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 
 
 
 
 
 


 


 


 
 
 
 
 
 

 

 

  

  

  

  

  

 

 

 


 
 
 
 
 
 

 

 

 

 

 

 

 

 
 
 
 
 
 


 

 

 

                                             
 

 
 
 
 
 
 

 

 

 

 

 

 

 

 

 

 
 

 

 
 
 

 

 

 


 
 
 
 
 
 

 


 



 

 

 

 

 

 

 

 
 
 
 
 
 

 

 

 



 
 
 
 
 
 


 

 

 

 

 
 
 
 
 
 





 


 






 
 


 


 


 


 


 

 





 




 

 
 


 

 

 

 
 


 


 

 

 


 

 

 

 

 



 
 

 

 



 



 
 
 
 
 
 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 


 
 
 
 
 
 

 



 

 



 

 

 

 

 

 

 

 

 



 

 

 

 

 

 

 
 
 
 
 

 



 

 

 

 

 

 

 







 

 
 
 
 
 
 
 

 


 



 



 



 

 



 



 



 



 

 



 



 



 



 

 



 



 



 



 

 

 
 
 
 
 
 




 



 






 

 

 

 








 





 








 





 

 

 

 

 

 

 

 

 

 



 


 

 



 
 
 
 
 
 

 

 

 

 



 

 


 
 
 
 
 
 
 


 
 

 


 


 
 
 
 
 
 

 


 
 

 

 
 
 
 
 
 

 


 







 

 

 


 

 

 


 

 


 



 

 


 

 
 

 

 

 

 

 

 

 

 

 

 



 



 

 

 

 

 

 

 




 

 

 



 





 




 

 

 

 

 

 

 

 

 

 


 
 



 

 
 






  



 



 

 

 


 

 

 

 

 


 
 


 


 



 

 

 

 

 

 

 

 

 

 

 

 

 
 

 

 

 

 

 

 

 


 

 

 

 

 

 

 
 
 

 

 

 

 
 

 

 

 

 

 

 

 

 



 









 



 



 





 



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













 








void platform_init(void);
void delay_ms(uint32_t delay_ms);
void delay_us(uint32_t delay_us);
uint32_t get_platform_counter(void);















 


 
 

  #pragma system_include

 
 

 

  #pragma system_include














 


 
 


  #pragma system_include

 
 

 

  #pragma system_include














 



 
  typedef _Sizet size_t;

typedef unsigned int __data_size_t;



 


 
  typedef   signed int ptrdiff_t;

  typedef   _Wchart wchar_t;


    typedef union
    {
      long long _ll;
      long double _ld;
      void *_vp;
    } _Max_align_t;
    typedef _Max_align_t max_align_t;






 

 
 
 
 
 
 
 



 



 


 
 



 
 



 

 
 



 


 
 


 
 


  #pragma system_include








 

 
enum usbd_machine_state {
    usbd_state_disabled,
    usbd_state_disconnected,
    usbd_state_default,          
    usbd_state_addressed,        
    usbd_state_configured,       
};

 
enum usbd_ctl_state {
    usbd_ctl_idle,               
    usbd_ctl_rxdata,             
    usbd_ctl_txdata,             
    usbd_ctl_ztxdata,           
 
    usbd_ctl_lastdata,          
 
    usbd_ctl_statusin,           
    usbd_ctl_statusout,          
};

 
typedef enum _usbd_respond {
    usbd_fail,                   
    usbd_ack,                    
    usbd_nak,                    
} usbd_respond;

typedef struct _usbd_device usbd_device;

 
typedef struct {
    uint8_t     bmRequestType;  
 
    uint8_t     bRequest;        
    uint16_t    wValue;         
 
    uint16_t    wIndex;         
 
    uint16_t    wLength;        
 
    uint8_t     data[];          
} usbd_ctlreq;

 
typedef struct {
    void        *data_buf;       
    void        *data_ptr;       
    uint16_t    data_count;      
    uint16_t    data_maxsize;    
    uint8_t     ep0size;         
    uint8_t     device_cfg;      
    uint8_t     device_state;    
    uint8_t     control_state;   
} usbd_status;






 
typedef void (*usbd_evt_callback)(usbd_device *dev, uint8_t event, uint8_t ep);





 
typedef void (*usbd_rqc_callback)(usbd_device *dev, usbd_ctlreq *req);















 
typedef usbd_respond (*usbd_ctl_callback)(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback);








 
typedef usbd_respond (*usbd_dsc_callback)(usbd_ctlreq *req, void **address, uint16_t *dsize);







 
typedef usbd_respond (*usbd_cfg_callback)(usbd_device *dev, uint8_t cfg);

 


 

 

typedef uint32_t (*usbd_hw_getinfo)(void);



 
typedef void (*usbd_hw_enable)(_Bool enable);




 
typedef uint8_t (*usbd_hw_connect)(_Bool connect);



 
typedef void (*usbd_hw_setaddr)(uint8_t address);






 
typedef _Bool (*usbd_hw_ep_config)(uint8_t ep, uint8_t eptype, uint16_t epsize);





 
typedef void (*usbd_hw_ep_deconfig)(uint8_t ep);






 
typedef int32_t (*usbd_hw_ep_read)(uint8_t ep, void *buf, uint16_t blen);






 
typedef int32_t (*usbd_hw_ep_write)(uint8_t ep, void *buf, uint16_t blen);





 
typedef void (*usbd_hw_ep_setstall)(uint8_t ep, _Bool stall);




 
typedef _Bool (*usbd_hw_ep_isstalled)(uint8_t ep);




 
typedef void (*usbd_hw_poll)(usbd_device *dev, usbd_evt_callback callback);

 
typedef uint16_t (*usbd_hw_get_frameno)(void);





 
typedef uint16_t (*usbd_hw_get_serialno)(void *buffer);

 
struct usbd_driver {
    usbd_hw_getinfo         getinfo;             
    usbd_hw_enable          enable;              
    usbd_hw_connect         connect;             
    usbd_hw_setaddr         setaddr;             
    usbd_hw_ep_config       ep_config;           
    usbd_hw_ep_deconfig     ep_deconfig;         
    usbd_hw_ep_read         ep_read;             
    usbd_hw_ep_write        ep_write;            
    usbd_hw_ep_setstall     ep_setstall;         
    usbd_hw_ep_isstalled    ep_isstalled;        
    usbd_hw_poll            poll;                
    usbd_hw_get_frameno     frame_no;            
    usbd_hw_get_serialno    get_serialno_desc;   
};

 


 

 
struct _usbd_device {
    const struct usbd_driver    *driver;                 
    usbd_ctl_callback           control_callback;        
    usbd_rqc_callback           complete_callback;       
    usbd_cfg_callback           config_callback;         
    usbd_dsc_callback           descriptor_callback;     
    usbd_evt_callback           events[8];  
    usbd_evt_callback           endpoint[8];             
    usbd_status                 status;                  
};







 
inline static void usbd_init(usbd_device *dev, const struct usbd_driver *drv,
                             const uint8_t ep0size, uint32_t *buffer, const uint16_t bsize) {
    dev->driver = drv;
    dev->status.ep0size = ep0size;
    dev->status.data_ptr = buffer;
    dev->status.data_buf = buffer;
    dev->status.data_maxsize = bsize - (__INTADDR__((&((usbd_ctlreq *)0)->data)));
}




 
void usbd_poll(usbd_device *dev);




 
inline static void usbd_reg_control(usbd_device *dev, usbd_ctl_callback callback) {
    dev->control_callback = callback;
}




 
inline static void usbd_reg_config(usbd_device *dev, usbd_cfg_callback callback) {
    dev->config_callback = callback;
}




 
inline static void usbd_reg_descr(usbd_device *dev, usbd_dsc_callback callback) {
    dev->descriptor_callback = callback;
}




 
inline static _Bool usbd_ep_config(usbd_device *dev, uint8_t ep, uint8_t eptype, uint16_t epsize) {
    return dev->driver->ep_config(ep, eptype, epsize);
}




 
inline static void usbd_ep_deconfig(usbd_device *dev, uint8_t ep) {
    dev->driver->ep_deconfig(ep);
}





 
inline static void usbd_reg_endpoint(usbd_device *dev, uint8_t ep, usbd_evt_callback callback) {
    dev->endpoint[ep & 0x07] = callback;
}





 
inline static void usbd_reg_event(usbd_device *dev, uint8_t evt, usbd_evt_callback callback) {
    dev->events[evt] = callback;
}




 
inline static int32_t usbd_ep_write(usbd_device *dev, uint8_t ep, void *buf, uint16_t blen) {
    return dev->driver->ep_write(ep, buf, blen);
}




 
inline static int32_t usbd_ep_read(usbd_device *dev, uint8_t ep, void *buf, uint16_t blen) {
    return dev->driver->ep_read(ep, buf, blen);
}




 
inline static void usbd_ep_stall(usbd_device *dev, uint8_t ep) {
    dev->driver->ep_setstall(ep, 1);
}




 
inline static void usbd_ep_unstall(usbd_device *dev, uint8_t ep) {
    dev->driver->ep_setstall(ep, 0);
}




 
inline static void usbd_enable(usbd_device *dev, _Bool enable) {
    dev->driver->enable(enable);
}





 
inline static uint8_t usbd_connect(usbd_device *dev, _Bool connect) {
    return dev->driver->connect(connect);
}


 
inline static uint32_t usbd_getinfo(usbd_device *dev) {
    return dev->driver->getinfo();
}

 
 















 









 


 




 

 
 

 
 


 
 



 
 


 
 


 
 


 
 


 
 


 
 


 
 


 
 

#pragma pack(push, 1)
 
struct usb_header_descriptor {
    uint8_t bLength;                 
    uint8_t bDescriptorType;         
};





 
struct usb_device_descriptor {
    uint8_t  bLength;                
    uint8_t  bDescriptorType;        
    uint16_t bcdUSB;                 
    uint8_t  bDeviceClass;           
    uint8_t  bDeviceSubClass;        
    uint8_t  bDeviceProtocol;        
    uint8_t  bMaxPacketSize0;        
    uint16_t idVendor;               
    uint16_t idProduct;              
    uint16_t bcdDevice;              
    uint8_t  iManufacturer;          
    uint8_t  iProduct;               
    uint8_t  iSerialNumber;          
    uint8_t  bNumConfigurations;     
};





 
struct usb_qualifier_descriptor {
    uint8_t  bLength;                
    uint8_t  bDescriptorType;        
    uint16_t bcdUSB;                 
    uint8_t  bDeviceClass;           
    uint8_t  bDeviceSubClass;        
    uint8_t  bDeviceProtocol;        
    uint8_t  bMaxPacketSize0;        
    uint8_t  bNumConfigurations;     
    uint8_t  bReserved;              
};




 
struct usb_config_descriptor {
    uint8_t  bLength;                
    uint8_t  bDescriptorType;        
    uint16_t wTotalLength;          
 
    uint8_t  bNumInterfaces;         
    uint8_t  bConfigurationValue;    
    uint8_t  iConfiguration;         
    uint8_t  bmAttributes;          


 
    uint8_t  bMaxPower;             
 
};




 
struct usb_interface_descriptor {
    uint8_t bLength;                 
    uint8_t bDescriptorType;         
    uint8_t bInterfaceNumber;        
    uint8_t bAlternateSetting;       
    uint8_t bNumEndpoints;           
    uint8_t bInterfaceClass;         
    uint8_t bInterfaceSubClass;      
    uint8_t bInterfaceProtocol;      
    uint8_t iInterface;              
};



 
struct usb_iad_descriptor {
    uint8_t bLength;                 
    uint8_t bDescriptorType;         
    uint8_t bFirstInterface;         
    uint8_t bInterfaceCount;         
    uint8_t bFunctionClass;          
    uint8_t bFunctionSubClass;       
    uint8_t bFunctionProtocol;       
    uint8_t iFunction;              
 
};



 
struct usb_endpoint_descriptor {
    uint8_t  bLength;                
    uint8_t  bDescriptorType;        
    uint8_t  bEndpointAddress;      
 
    uint8_t  bmAttributes;           
    uint16_t wMaxPacketSize;        
 
    uint8_t  bInterval;             
 
};






 
struct usb_string_descriptor {
    uint8_t  bLength;                
    uint8_t  bDescriptorType;        
    uint16_t wString[];             
 
};





 
struct usb_debug_descriptor {
    uint8_t  bLength;                
    uint8_t  bDescriptorType;        
    uint8_t  bDebugInEndpoint;       
    uint8_t  bDebugOutEndpoint;      
};
#pragma pack(pop)

 













 


void usbd_hw_init(usbd_device *dev);













 


































#pragma pack(push, 1)


struct usb_uac_ac_header_sz1_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint16_t bcdADC;
	uint16_t wTotalLength;
	uint8_t bInCollection;
	uint8_t baInterfaceNr[1];
};


struct usb_uac_input_terminal_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t bNrChannels;
	uint16_t wChannelConfig;
	uint8_t iChannelNames;
	uint8_t iTerminal;
};


struct usb_uac_feature_unit_sz2x16_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint8_t bControlSize;
	uint16_t bmaControls[2];
	uint8_t iFeature;
};
struct usb_uac_feature_unit_sz3x16_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint8_t bControlSize;
	uint16_t bmaControls[3];
	uint8_t iFeature;
};


struct usb_uac_output_terminal_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalID;
	uint16_t wTerminalType;
	uint8_t bAssocTerminal;
	uint8_t bSourceID;
	uint8_t iTerminal;
};


struct usb_uac_as_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bTerminalLink;
	uint8_t bDelay;
	uint16_t wFormatTag;
};


struct usb_uac_as_format_type1_sz3_desc
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bFormatType;
	uint8_t bNrChannels;
	uint8_t bSubframeSize;
	uint8_t bBitResolution;
	uint8_t bSamFreqType;
	uint8_t tSamFreq[3];
};


struct usb_uac_std_iso_endpoint_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
	uint8_t bRefresh;
	uint8_t bSynchAddress;
};


struct usb_uac_cs_iso_endpoint_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmAttributes;
	uint8_t bLockDelayUnits;
	uint16_t wLockDelay;
};


struct usb_uac_form1_mute_control_parameter_block
{
	uint8_t bMute;
};


struct usb_uac_form2_mute_control_parameter_sz2_block
{
	uint8_t bMute[2];
};
struct usb_uac_form2_mute_control_parameter_sz3_block
{
	uint8_t bMute[3];
};

#pragma pack(pop)














 


typedef void (*write_dma_txbuf_complete_callback)(void);
typedef void (*mclk_count_complete_callback)(uint32_t mclk);

typedef struct audio_out_drv
{
	void (*init)(void);
	void (*init_dma_txbuf)(write_dma_txbuf_complete_callback callback);
	void (*write_dma_txbuf)(void *txbuf, uint32_t length);
	double (*get_mclk)(void);
	void (*mclk_set_sof)(uint32_t sof);
	void (*mclk_start_count)(mclk_count_complete_callback callback);
	void (*mclk_stop_count)(void);
	void (*convert)(uint32_t *in, uint32_t *out);
} audio_out_drv_t;




extern const audio_out_drv_t audio_out_drv;

extern const struct usbd_driver usbd_otgfs;

















#pragma pack(push, 1)
typedef struct uac_config
{
	struct usb_config_descriptor                        config;
	struct usb_iad_descriptor                           iad;
	struct usb_interface_descriptor                     ac;
	struct usb_uac_ac_header_sz1_desc                   ac_hdr;
	struct usb_uac_input_terminal_desc                  ac_itd;
	struct usb_uac_feature_unit_sz3x16_desc             ac_fu;
	struct usb_uac_output_terminal_desc                 ac_otd;
	struct usb_interface_descriptor                     as_0;
	struct usb_interface_descriptor                     as_1;
	struct usb_uac_as_desc                              as_hdr;
	struct usb_uac_as_format_type1_sz3_desc             as_format_type1;
	struct usb_uac_std_iso_endpoint_descriptor          eprx_data_std;
	struct usb_uac_cs_iso_endpoint_descriptor           eprx_data_cs;
	struct usb_uac_std_iso_endpoint_descriptor          eptx_fback_std;
} uac_config_t;
#pragma pack(pop)


static const struct usb_device_descriptor device_desc =
{
	.bLength            = sizeof(struct usb_device_descriptor),
	.bDescriptorType    = 0x01,
	.bcdUSB             = ((((2 & 0xFF) << 8) | ((0 & 0x0F) << 4) | (0 & 0x0F))),
	.bDeviceClass       = 0xEF,
	.bDeviceSubClass    = 0x02,
	.bDeviceProtocol    = 0x01,
	.bMaxPacketSize0    = 64,
	.idVendor           = (0x0483),
	.idProduct          = (0x5740),
	.bcdDevice          = ((((1 & 0xFF) << 8) | ((0 & 0x0F) << 4) | (0 & 0x0F))),
	.iManufacturer      = 1,
	.iProduct           = 2,
	.iSerialNumber      = 0xFE,
	.bNumConfigurations = 1,
};


static const uac_config_t config_desc =
{
	.config =
	{
		.bLength                   = sizeof(struct usb_config_descriptor),
		.bDescriptorType           = 0x02,
		.wTotalLength              = (sizeof(uac_config_t)),
		.bNumInterfaces            = 2,
		.bConfigurationValue       = 1,
		.iConfiguration            = 0x00,
		.bmAttributes              = 0x80 | 0x40,
		.bMaxPower                 = ((100) >> 1),
	},
	.iad =
	{
		.bLength                   = sizeof(struct usb_iad_descriptor),
		.bDescriptorType           = 0x0B,
		.bFirstInterface           = 0,
		.bInterfaceCount           = 2,
		.bFunctionClass            = 0x01,
		.bFunctionSubClass         = 0x00,
		.bFunctionProtocol         = 0x00,
		.iFunction                 = 2,
	},
	.ac =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = 0x04,
		.bInterfaceNumber          = 0,
		.bAlternateSetting         = 0,
		.bNumEndpoints             = 0,
		.bInterfaceClass           = 0x01,
		.bInterfaceSubClass        = 0x01,
		.bInterfaceProtocol        = 0x00,
		.iInterface                = 0x00,
	},
	.ac_hdr =
	{
		.bLength                   = sizeof(struct usb_uac_ac_header_sz1_desc),
		.bDescriptorType           = 0x24,
		.bDescriptorSubType        = 0x01,
		.bcdADC                    = ((((1 & 0xFF) << 8) | ((0 & 0x0F) << 4) | (0 & 0x0F))),
		.wTotalLength              = (sizeof(struct usb_uac_ac_header_sz1_desc) + sizeof(struct usb_uac_input_terminal_desc) + sizeof(struct usb_uac_feature_unit_sz3x16_desc) + sizeof(struct usb_uac_output_terminal_desc)),
		.bInCollection             = 1,
		.baInterfaceNr             = 1,
	},
	.ac_itd =
	{
		.bLength                   = sizeof(struct usb_uac_input_terminal_desc),
		.bDescriptorType           = 0x24,
		.bDescriptorSubType        = 0x02,
		.bTerminalID               = 1,
		.wTerminalType             = (0x0101),
		.bAssocTerminal            = 0,
		.bNrChannels               = 2,
		.wChannelConfig            = (0x0003), 
		.iChannelNames             = 0,
		.iTerminal                 = 0,
	},
	.ac_fu =
	{
		.bLength                   = sizeof(struct usb_uac_feature_unit_sz3x16_desc),
		.bDescriptorType           = 0x24,
		.bDescriptorSubType        = 0x06,
		.bUnitID                   = 2,
		.bSourceID                 = 1,
		.bControlSize              = 2,
		.bmaControls[0]            = (0x0000), 
		.bmaControls[1]            = (0x0001), 
		.bmaControls[2]            = (0x0001), 
		.iFeature                  = 0,
	},
	.ac_otd =
	{
		.bLength                   = sizeof(struct usb_uac_output_terminal_desc),
		.bDescriptorType           = 0x24,
		.bDescriptorSubType        = 0x03,
		.bTerminalID               = 3,
		.wTerminalType             = (0x0302),
		.bAssocTerminal            = 0,
		.bSourceID                 = 2,
		.iTerminal                 = 0,
	},
	.as_0 =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = 0x04,
		.bInterfaceNumber          = 1,
		.bAlternateSetting         = 0,
		.bNumEndpoints             = 0,
		.bInterfaceClass           = 0x01,
		.bInterfaceSubClass        = 0x02,
		.bInterfaceProtocol        = 0x00,
		.iInterface                = 0x00,
	},
	.as_1 =
	{
		.bLength                   = sizeof(struct usb_interface_descriptor),
		.bDescriptorType           = 0x04,
		.bInterfaceNumber          = 1,
		.bAlternateSetting         = 1,
		.bNumEndpoints             = 2,
		.bInterfaceClass           = 0x01,
		.bInterfaceSubClass        = 0x02,
		.bInterfaceProtocol        = 0x00,
		.iInterface                = 0x00,
	},
	.as_hdr =
	{
		.bLength                   = sizeof(struct usb_uac_as_desc),
		.bDescriptorType           = 0x24,
		.bDescriptorSubType        = 0x01,
		.bTerminalLink             = 1,
		.bDelay                    = 0,
		.wFormatTag                = (0x0001),
	},
	.as_format_type1 =
	{
		.bLength                   = sizeof(struct usb_uac_as_format_type1_sz3_desc),
		.bDescriptorType           = 0x24,
		.bDescriptorSubType        = 0x02,
		.bFormatType               = 0x01,
		.bNrChannels               = 2,
		.bSubframeSize             = 2,
		.bBitResolution            = 16,
		.bSamFreqType              = 0x01,
		.tSamFreq[0]               = (uint8_t)(48000),
		.tSamFreq[1]               = (uint8_t)(48000 >> 8),
		.tSamFreq[2]               = (uint8_t)(48000 >> 16),
	},
	.eprx_data_std =
	{
		.bLength                   = sizeof(struct usb_uac_std_iso_endpoint_descriptor),
		.bDescriptorType           = 0x05,
		.bEndpointAddress          = 0x01,
		.bmAttributes              = 0x01 | 0x04,
		.wMaxPacketSize            = ((((48000 / 1000) * 1 * 2 * 2) + 2 * 2 * 1)),
		.bInterval                 = 1,
		.bRefresh                  = 0,
		.bSynchAddress             = 0x82,
	},
	.eprx_data_cs =
	{
		.bLength                   = sizeof(struct usb_uac_cs_iso_endpoint_descriptor),
		.bDescriptorType           = 0x25,
		.bDescriptorSubType        = 0x01,
		.bmAttributes              = 0x00,
		.bLockDelayUnits           = 0,
		.wLockDelay                = (0x0000),
	},
	.eptx_fback_std =
	{
		.bLength                   = sizeof(struct usb_uac_std_iso_endpoint_descriptor),
		.bDescriptorType           = 0x05,
		.bEndpointAddress          = 0x82,
		.bmAttributes              = 0x01 | 0x10,
		.wMaxPacketSize            = (4),
		.bInterval                 = 1,
		.bRefresh                  = 5,
		.bSynchAddress             = 0,
	},
};


static const struct usb_string_descriptor lang_desc = {. bLength = 2 + sizeof((uint16_t[]){0x0409}), . bDescriptorType = 0x03, . wString = {0x0409}};
static const struct usb_string_descriptor manuf_desc_en = {. bLength = sizeof(u"Homemcu firmware examples"), . bDescriptorType = 0x03, . wString = {u"Homemcu firmware examples"}};
static const struct usb_string_descriptor prod_desc_en = {. bLength = sizeof(u"UAC I2S-DAC demo"), . bDescriptorType = 0x03, . wString = {u"UAC I2S-DAC demo"}};
static const struct usb_string_descriptor *const dtable[] =
{
	&lang_desc,
	&manuf_desc_en,
	&prod_desc_en,
};


typedef struct audio_state
{
	uint8_t mute[2 + 1];
	_Bool playback;
	_Bool feedback;
	_Bool start_usb;
	_Bool start_i2s;
	uint8_t cnt_i2s;
	uint8_t cnt_usb;
	uint32_t total_cnt_i2s;
	uint32_t total_cnt_usb;
} audio_state_t;

typedef struct feedback_state
{
	uint32_t mclk_count_measured;
	uint32_t mclk_count_expected;
	uint32_t mclk_count_correction;
	uint8_t feedback_data[4];
	uint32_t flagO;
	uint32_t flagE;
	uint32_t flagU;
	uint32_t flagU1;
	uint32_t flagU2;
	uint32_t flagU3;
	uint32_t flagU4;
} feedback_state_t;


static usbd_device udev;
static uint8_t iface_num;
static uint8_t altset_num;
static audio_state_t audio;
static feedback_state_t feedback;

static uint32_t ubuf[(128 + 3) / sizeof(uint32_t)];
static uint32_t buff_usb[16][((((48000 / 1000) * 1 * 2 * 2) + 2 * 2 * 1) + 3) / sizeof(uint32_t)];
static uint16_t buff_usb_size[16];

static uint32_t buff_uac_ctrl[(16 + 3) / sizeof(uint32_t)];









































static void mclk_callback(uint32_t mclk)
{
	uint32_t feedback_1014;

	feedback.mclk_count_measured = mclk;
	if (audio.total_cnt_usb > audio.total_cnt_i2s)
	{
		++feedback.flagU;
		if (audio.total_cnt_usb - audio.total_cnt_i2s >= 16 - 16 / 8)
		{
			feedback.mclk_count_measured -= feedback.mclk_count_correction * 2;
			++feedback.flagU4;
		}
		if (audio.total_cnt_usb - audio.total_cnt_i2s >= 16 - 16 / 4)
		{
			feedback.mclk_count_measured -= feedback.mclk_count_correction;
			++feedback.flagU3;
		}
		if (audio.total_cnt_usb - audio.total_cnt_i2s <= 16 / 4)
		{
			feedback.mclk_count_measured += feedback.mclk_count_correction;
			++feedback.flagU2;
		}
		if (audio.total_cnt_usb - audio.total_cnt_i2s <= 16 / 8)
		{
			feedback.mclk_count_measured += feedback.mclk_count_correction * 2;
			++feedback.flagU1;
		}
	}
	else
	{
		++feedback.flagO;
		feedback.mclk_count_measured += feedback.mclk_count_correction * 4;
	}

	feedback_1014 = (feedback . mclk_count_measured << (14 - (5 + 8)));
	feedback.feedback_data[0] = feedback_1014 & 0xFF;
	feedback.feedback_data[1] = (feedback_1014 >> 8) & 0xFF;
	feedback.feedback_data[2] = (feedback_1014 >> 16) & 0xFF;
	feedback.feedback_data[3] = (feedback_1014 >> 24) & 0xFF;

	audio.feedback = 1;
}


static void start_mclk_count(void)
{
	feedback.mclk_count_expected = (uint32_t)(audio_out_drv.get_mclk() / 1000 * (1 << 5));
	feedback.mclk_count_correction = feedback.mclk_count_expected / 2048;
	audio.feedback = 0;
	audio_out_drv.mclk_set_sof((1 << 5) - 1);
	audio_out_drv.mclk_start_count(mclk_callback);
}


static void stop_mclk_count(void)
{
	audio_out_drv.mclk_stop_count();
	audio.feedback = 0;
}



static usbd_respond uac_getdesc(usbd_ctlreq *req, void **address, uint16_t *length)
{
	uint8_t dtype = req->wValue >> 8;
	uint8_t dnumber = req->wValue;
	const void* desc;
	uint16_t len = 0;

	switch (dtype)
	{
	case 0x01:
		desc = &device_desc;
		break;
	case 0x02:
		desc = &config_desc;
		len = sizeof(config_desc);
		break;
	case 0x03:
		if (dnumber < sizeof(dtable) / sizeof(dtable[0]))
		{
			desc = dtable[dnumber];
		}
		else
		{
			return usbd_fail;
		}
		break;
	default:
		return usbd_fail;
	}
	if (len == 0)
	{
		len = ((struct usb_header_descriptor*)desc)->bLength;
	}
	*address = (void*)desc;
	*length = len;
	return usbd_ack;
};


static usbd_respond uac_feature_control(usbd_device *dev, usbd_ctlreq *req)
{
	uint8_t length;
	
	uint8_t cn = req->wValue & 0xFF;
	
	uint8_t cs = req->wValue >> 8;

	switch (cs)
	{
	case 0x01:
		{
			
			switch (req->bRequest)
			{
			case 0x81:
				if (cn == 0xFF)
				{
					struct usb_uac_form2_mute_control_parameter_sz3_block *block = (struct usb_uac_form2_mute_control_parameter_sz3_block *)&buff_uac_ctrl[0];
					block->bMute[0] = audio.mute[0];
					block->bMute[1] = audio.mute[1];
					block->bMute[2] = audio.mute[2];
					length = sizeof(struct usb_uac_form2_mute_control_parameter_sz3_block);
				}
				else
				{
					struct usb_uac_form1_mute_control_parameter_block *block = (struct usb_uac_form1_mute_control_parameter_block *)&buff_uac_ctrl[0];
					block->bMute = audio.mute[0];
					length = sizeof(struct usb_uac_form1_mute_control_parameter_block);
				}
				dev->status.data_ptr = &buff_uac_ctrl[0];
				dev->status.data_count = length < req->wLength ? length : req->wLength;
				return usbd_ack;
			case 0x01:
				if (cn == 0xFF)
				{
					audio.mute[0] = req->data[0];
					audio.mute[1] = req->data[1];
					audio.mute[2] = req->data[2];
				}
				else
				{
					audio.mute[cn] = req->data[0];
				}
				return usbd_ack;
			}
		}
	}
	return usbd_fail;
}


static usbd_respond uac_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
{
	if ((((3 << 0) | (3 << 5)) & req->bmRequestType) != ((1 << 0) | (1 << 5)))
	{
		if (req->bRequest == 0x0A)
		{
			if (req->wIndex == 1)
			{
				dev->status.data_ptr = &altset_num;
				dev->status.data_count = 1;
				return usbd_ack;
			}
		}
		if (req->bRequest == 0x0B)
		{
			altset_num = req->wValue;
			iface_num = req->wIndex;
			if (iface_num == 1 && altset_num == 1)
			{
				if (!audio.playback)
				{
					audio.cnt_i2s = 0;
					audio.cnt_usb = 0;
					audio.total_cnt_i2s = 0;
					audio.total_cnt_usb = 0;
					audio.start_i2s = 1;
					audio.start_usb = 1;
					start_mclk_count();
					usbd_ep_write(dev, 0x82, (void *)0, 0);
				}
			}
			if (iface_num == 1 && altset_num == 0)
			{
				if (audio.playback)
				{
					stop_mclk_count();
					audio.playback = 0;
				}
			}
			return usbd_ack;
		}
		return usbd_fail;
	}

	
	uint8_t iface = req->wIndex & 0xFF;
	
	uint8_t entity_id = req->wIndex >> 8;

	switch (iface)
	{
	case 0:
		{
			switch (entity_id)
			{
			case 2:
				return uac_feature_control(dev, req);
			}
		}
	}
	return usbd_fail;
}


static void uac_callback(usbd_device *dev, uint8_t event, uint8_t ep)
{
	switch (event)
	{
	case 5:
		if (!audio.start_usb)
		{
			audio.playback = 1;
			++audio.total_cnt_usb;
			++audio.cnt_usb;
			if (audio.cnt_usb == 16)
			{
				audio.cnt_usb = 0;
			}
		}
		audio.start_usb = 0;
		buff_usb_size[audio.cnt_usb] = usbd_ep_read(dev, 0x01, &buff_usb[audio.cnt_usb][0], (((48000 / 1000) * 1 * 2 * 2) + 2 * 2 * 1));


		if (audio.start_i2s && audio.cnt_usb >= 16 / 2)
		{
			audio.start_i2s = 0;
			audio_out_drv.write_dma_txbuf(&buff_usb[audio.cnt_i2s][0], buff_usb_size[audio.cnt_i2s]);
		}
		break;
	case 4:
		if (audio.feedback)
		{
			usbd_ep_write(dev, 0x82, &feedback.feedback_data[0], sizeof(feedback.feedback_data));
			audio.feedback = 0;
		}
		else
		{
			usbd_ep_write(dev, 0x82, (void *)0, 0);
		}
		break;
	default:
		break;
	}
}


static void i2s_tx_complete_callback(void)
{
	if (audio.playback)
	{
		if (audio.total_cnt_i2s < audio.total_cnt_usb)
		{
			++audio.total_cnt_i2s;
			++audio.cnt_i2s;
			if (audio.cnt_i2s == 16)
			{
				audio.cnt_i2s = 0;
			}
		}
		else
		{
			++feedback.flagE;
		}
		audio_out_drv.write_dma_txbuf(&buff_usb[audio.cnt_i2s][0], buff_usb_size[audio.cnt_i2s]);
	}
}


static usbd_respond uac_setconf(usbd_device *dev, uint8_t cfg)
{
	switch (cfg)
	{
	case 0:
		
		usbd_ep_deconfig(dev, 0x01);
		usbd_ep_deconfig(dev, 0x82);
		usbd_reg_endpoint(dev, 0x01, 0);
		usbd_reg_endpoint(dev, 0x82, 0);
		return usbd_ack;
	case 1:
        
		usbd_ep_config(dev, 0x01, 0x01, (((48000 / 1000) * 1 * 2 * 2) + 2 * 2 * 1));
		usbd_ep_config(dev, 0x82, 0x01, 4);
		usbd_reg_endpoint(dev, 0x01, uac_callback);
		usbd_reg_endpoint(dev, 0x82, uac_callback);
		return usbd_ack;
	default:
		return usbd_fail;
	}
}


void usb_uac_i2s_init(void)
{
	audio_out_drv.init();
	audio_out_drv.init_dma_txbuf(i2s_tx_complete_callback);

	usbd_hw_init(&udev);
	usbd_init(&udev, &usbd_otgfs, 64, ubuf, sizeof(ubuf));
	usbd_reg_control(&udev, uac_control);
	usbd_reg_config(&udev, uac_setconf);
	usbd_reg_descr(&udev, uac_getdesc);
}


void usb_uac_i2s_loop(void)
{
	usbd_enable(&udev, 1);
	usbd_connect(&udev, 1);
	while (1);
}
