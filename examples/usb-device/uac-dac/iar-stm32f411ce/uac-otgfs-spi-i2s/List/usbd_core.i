













 

 
 

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

 






















 











 
 
 


  #pragma system_include








 
 
 

  #pragma system_include

 
 

 

  #pragma system_include














 


 
 


  #pragma system_include

 
 

 

  #pragma system_include














 



 
  typedef _Sizet size_t;

typedef unsigned int __data_size_t;











 


  #pragma system_include


  





 


  




 




  


 


  #pragma inline=forced_no_body
  _Pragma("function_effects = no_state, no_read(1), no_write(2), returns 1, always_returns") __intrinsic __nounwind void * memcpy(void * _D, const void * _S, size_t _N)
  {
    __aeabi_memcpy(_D, _S, _N);
    return _D;
  }

  #pragma inline=forced_no_body
  _Pragma("function_effects = no_state, no_read(1), no_write(2), returns 1, always_returns") __intrinsic __nounwind void * memmove(void * _D, const void * _S, size_t _N)
  {
    __aeabi_memmove(_D, _S, _N);
    return _D;
  }

  #pragma inline=forced_no_body
  _Pragma("function_effects = no_state, no_read(1), returns 1, always_returns") __intrinsic __nounwind void * memset(void * _D, int _C, size_t _N)
  {
    __aeabi_memset(_D, _N, _C);
    return _D;
  }




 

 

 

  _Pragma("function_effects = no_state, no_write(1,2), always_returns")   __intrinsic __nounwind   int       memcmp(const void *, const void *,
                                                   size_t);
  _Pragma("function_effects = no_state, no_read(1), no_write(2), returns 1, always_returns")  __intrinsic __nounwind void *    memcpy(void *restrict,
                                                   const void *restrict,
                                                   size_t);
  _Pragma("function_effects = no_state, no_read(1), no_write(2), returns 1, always_returns")  __intrinsic __nounwind void *    memmove(void *, const void *,
                                                    size_t);
  _Pragma("function_effects = no_state, no_read(1), returns 1, always_returns")     __intrinsic __nounwind void *    memset(void *, int, size_t);
  _Pragma("function_effects = no_state, no_write(2), returns 1, always_returns")     __intrinsic __nounwind char *    strcat(char *restrict,
                                                   const char *restrict);
  _Pragma("function_effects = no_state, no_write(1,2), always_returns")   __intrinsic __nounwind   int       strcmp(const char *, const char *);
  _Pragma("function_effects = no_write(1,2), always_returns")     __intrinsic __nounwind   int       strcoll(const char *, const char *);
  _Pragma("function_effects = no_state, no_read(1), no_write(2), returns 1, always_returns")  __intrinsic __nounwind char *    strcpy(char *restrict,
                                                   const char *restrict);
  _Pragma("function_effects = no_state, no_write(1,2), always_returns")   __intrinsic __nounwind   size_t    strcspn(const char *, const char *);
                    __intrinsic __nounwind char *    strerror(int);
  _Pragma("function_effects = no_state, no_write(1), always_returns")      __intrinsic __nounwind   size_t    strlen(const char *);
  _Pragma("function_effects = no_state, no_write(2), returns 1, always_returns")     __intrinsic __nounwind char *    strncat(char *restrict,
                                                    const char *restrict,
                                                    size_t);
  _Pragma("function_effects = no_state, no_write(1,2), always_returns")   __intrinsic __nounwind   int       strncmp(const char *, const char *,
                                                    size_t);
  _Pragma("function_effects = no_state, no_read(1), no_write(2), returns 1, always_returns")  __intrinsic __nounwind char *    strncpy(char *restrict,
                                                    const char *restrict,
                                                    size_t);
  _Pragma("function_effects = no_state, no_write(1,2), always_returns")   __intrinsic __nounwind   size_t    strspn(const char *, const char *);
  _Pragma("function_effects = no_write(2), always_returns")         __intrinsic __nounwind char *    strtok(char *restrict,
                                                   const char *restrict);
  _Pragma("function_effects = no_write(2), always_returns")        __intrinsic __nounwind   size_t    strxfrm(char *restrict,
                                                    const char *restrict,
                                                    size_t);
    _Pragma("function_effects = no_write(1), always_returns")      __intrinsic __nounwind   char *    strdup(const char *);
    _Pragma("function_effects = no_write(1,2), always_returns")   __intrinsic __nounwind   int       strcasecmp(const char *,
                                                       const char *);
    _Pragma("function_effects = no_write(1,2), always_returns")   __intrinsic __nounwind   int       strncasecmp(const char *,
                                                        const char *, size_t);
    _Pragma("function_effects = no_state, no_write(2), always_returns")    __intrinsic __nounwind   char *    strtok_r(char *, const char *,
                                                     char **);
    _Pragma("function_effects = no_state, no_write(1), always_returns")     __intrinsic __nounwind size_t    strnlen(char const *, size_t);


  _Pragma("function_effects = no_state, no_write(1), always_returns")    __intrinsic __nounwind void *memchr(const void *_S, int _C, size_t _N);
  _Pragma("function_effects = no_state, no_write(1), always_returns")    __intrinsic __nounwind char *strchr(const char *_S, int _C);
  _Pragma("function_effects = no_state, no_write(1,2), always_returns") __intrinsic __nounwind char *strpbrk(const char *_S, const char *_P);
  _Pragma("function_effects = no_state, no_write(1), always_returns")    __intrinsic __nounwind char *strrchr(const char *_S, int _C);
  _Pragma("function_effects = no_state, no_write(1,2), always_returns") __intrinsic __nounwind char *strstr(const char *_S, const char *_P);






 














 


 
 

  #pragma system_include

 
 

 

  #pragma system_include














 



 


 
  typedef   signed int ptrdiff_t;

  typedef   _Wchart wchar_t;


    typedef union
    {
      long long _ll;
      long double _ld;
      void *_vp;
    } _Max_align_t;
    typedef _Max_align_t max_align_t;






 

 
 
 
 
 
 
 



 



 


 
 



 
 



 

 
 



 


 
 



 
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

 



static void usbd_process_ep0 (usbd_device *dev, uint8_t event, uint8_t ep);




 
static void usbd_process_reset(usbd_device *dev) {
    dev->status.device_state = usbd_state_default;
    dev->status.control_state = usbd_ctl_idle;
    dev->status.device_cfg = 0;
    dev->driver->ep_config(0, 0x00, dev->status.ep0size);
    dev->endpoint[0] = usbd_process_ep0;
    dev->driver->setaddr(0);
}





 
static void usbd_set_address (usbd_device *dev, usbd_ctlreq *req) {
    dev->driver->setaddr(req->wValue);
    dev->status.device_state = (req->wValue) ? usbd_state_addressed : usbd_state_default;
}




 
static void usbd_process_callback (usbd_device *dev) {
    if (dev->complete_callback) {
        dev->complete_callback(dev, dev->status.data_buf);
        dev->complete_callback = 0;
    }
}





 
static usbd_respond usbd_configure(usbd_device *dev, uint8_t config) {
    if (dev->config_callback) {
        if (dev->config_callback(dev, config) == usbd_ack) {
            dev->status.device_cfg = config;
            dev->status.device_state = (config) ? usbd_state_configured : usbd_state_addressed;
            return usbd_ack;
        }
    }
    return usbd_fail;
}






 
static usbd_respond usbd_process_devrq (usbd_device *dev, usbd_ctlreq *req) {
    switch (req->bRequest) {
    case 0x01:
         
        break;
    case 0x08:
        req->data[0] = dev->status.device_cfg;
        return usbd_ack;
    case 0x06:
        if (req->wValue == ((0x03 << 8) | 0xFE )) {
            dev->status.data_count = dev->driver->get_serialno_desc(req->data);
            return usbd_ack;
        } else {
            if (dev->descriptor_callback) {
                return dev->descriptor_callback(req, &(dev->status.data_ptr), &(dev->status.data_count));
            }
        }
        break;
    case 0x00:
        req->data[0] = 0;
        req->data[1] = 0;
        return usbd_ack;
    case 0x05:
        if (usbd_getinfo(dev) & (1 << 0)) {
            usbd_set_address(dev, req);
        } else {
            dev->complete_callback = usbd_set_address;
        }
        return usbd_ack;
    case 0x09:
        return usbd_configure(dev, req->wValue);
    case 0x07:
         
        break;
    case 0x03:
         
        break;
    default:
        break;
    }
    return usbd_fail;
}





 
static usbd_respond usbd_process_intrq(usbd_device *dev, usbd_ctlreq *req) {
    (void)dev;
    switch (req->bRequest) {
    case 0x00:
        req->data[0] = 0;
        req->data[1] = 0;
        return usbd_ack;
    default:
        break;
    }
    return usbd_fail;
}





 
static usbd_respond usbd_process_eptrq(usbd_device *dev, usbd_ctlreq *req) {
    switch (req->bRequest) {
    case 0x03:
        dev->driver->ep_setstall(req->wIndex, 1);
        return usbd_ack;
    case 0x01:
        dev->driver->ep_setstall(req->wIndex, 0);
        return usbd_ack;
    case 0x00:
        req->data[0] = dev->driver->ep_isstalled(req->wIndex) ? 1 : 0;
        req->data[1] = 0;
        return usbd_ack;
    default:
        break;
    }
    return usbd_fail;
}





 
static usbd_respond usbd_process_request(usbd_device *dev, usbd_ctlreq *req) {
     
    if (dev->control_callback) {
        usbd_respond r = dev->control_callback(dev, req, &(dev->complete_callback));
        if (r != usbd_fail) return r;
    }
     
    switch (req->bmRequestType & ((3 << 5) | (3 << 0))) {
    case (0 << 5) | (0 << 0):
        return usbd_process_devrq(dev, req);
    case (0 << 5) | (1 << 0):
        return usbd_process_intrq(dev, req);
    case (0 << 5) | (2 << 0):
        return usbd_process_eptrq(dev, req);
    default:
        break;
    }
    return usbd_fail;
}





 
static void usbd_stall_pid(usbd_device *dev, uint8_t ep) {
    dev->driver->ep_setstall(ep & 0x7F, 1);
    dev->driver->ep_setstall(ep | 0x80, 1);
    dev->status.control_state = usbd_ctl_idle;
}





 
static void usbd_process_eptx(usbd_device *dev, uint8_t ep) {
    int32_t _t;
    switch (dev->status.control_state) {
    case usbd_ctl_ztxdata:
    case usbd_ctl_txdata:
        _t = ((dev->status . data_count) < (dev->status . ep0size)) ? (dev->status . data_count) : (dev->status . ep0size);
        dev->driver->ep_write(ep, dev->status.data_ptr, _t);
        dev->status.data_ptr = (void *)((uint8_t *)dev->status.data_ptr + _t);
        dev->status.data_count -= _t;
         
        if (0 != dev->status.data_count) break;
         
         
        if ( usbd_ctl_txdata == dev->status.control_state || _t != dev->status.ep0size ) {
            dev->status.control_state = usbd_ctl_lastdata;  
        }
        break;
    case usbd_ctl_lastdata:
        dev->status.control_state = usbd_ctl_statusout;
        break;
    case usbd_ctl_statusin:
        dev->status.control_state = usbd_ctl_idle;
        usbd_process_callback(dev);
        return;
    default:
         
         
        break;
    }
}




 
static void usbd_process_eprx(usbd_device *dev, uint8_t ep) {
    uint16_t _t;
    usbd_ctlreq *const req = dev->status.data_buf;
    switch (dev->status.control_state) {
    case usbd_ctl_idle:
         
        if (0x08 !=  dev->driver->ep_read(ep, req, dev->status.data_maxsize)) {
            usbd_stall_pid(dev, ep);
            return;
        }
        dev->status.data_ptr = req->data;
        dev->status.data_count = req->wLength;
         
        if ((req->bmRequestType & (1 << 7)) || (0 == req->wLength)) {
            break;
        }
         
        if (req->wLength > dev->status.data_maxsize) {
            usbd_stall_pid(dev, ep);
            return;
        }
         
        dev->status.control_state = usbd_ctl_rxdata;
        return;
    case usbd_ctl_rxdata:
         
        _t = dev->driver->ep_read(ep, dev->status.data_ptr, dev->status.data_count);
        if (dev->status.data_count < _t) {
         
         
            usbd_stall_pid(dev, ep);
            return;
        } else if (dev->status.data_count != _t) {
         
            dev->status.data_count -= _t;
            dev->status.data_ptr = (void *)((uint8_t *)dev->status.data_ptr + _t);
            return;
        }
        break;
    case usbd_ctl_statusout:
         
        dev->driver->ep_read(ep, 0, 0);
        dev->status.control_state = usbd_ctl_idle;
        usbd_process_callback(dev);
        return;
    default:
         
        usbd_stall_pid(dev, ep);
        return;
    }
     
    dev->status.data_ptr = req->data;
    dev->status.data_count =  dev->status.data_maxsize;
    switch (usbd_process_request(dev, req)) {
    case usbd_ack:
        if (req->bmRequestType & (1 << 7)) {
             
            if (dev->status.data_count >= req->wLength) {
                dev->status.data_count = req->wLength;
                dev->status.control_state = usbd_ctl_txdata;
            } else {
                 
                 
                dev->status.control_state = usbd_ctl_ztxdata;
            }
              usbd_process_eptx(dev, ep | 0x80);
              return;

        } else {
             
            dev->driver->ep_write(ep | 0x80, 0, 0);
            dev->status.control_state = usbd_ctl_statusin;
        }
        break;
    case usbd_nak:
        dev->status.control_state = usbd_ctl_statusin;
        break;
    default:
          usbd_stall_pid(dev, ep);
          return;
    }
}




 
static void usbd_process_ep0 (usbd_device *dev, uint8_t event, uint8_t ep) {
    switch (event) {
    case 6:
         
        dev->status.control_state = usbd_ctl_idle;
        dev->complete_callback = 0;
    case 5:
        usbd_process_eprx(dev, ep);
        return;
    case 4:
        usbd_process_eptx(dev, ep);
        return;
    default:
        break;
    }
}






 
static void usbd_process_evt(usbd_device *dev, uint8_t evt, uint8_t ep) {
    switch (evt) {
    case 0:
        usbd_process_reset(dev);
        break;
    case 5:
    case 4:
    case 6:
        if (dev->endpoint[ep & 0x07]) dev->endpoint[ep & 0x07](dev, evt, ep);
        break;
    default:
        break;
    }
    if (dev->events[evt]) dev->events[evt](dev, evt, ep);
}

void usbd_poll(usbd_device *dev) {
    dev->driver->poll(dev, usbd_process_evt);
}
