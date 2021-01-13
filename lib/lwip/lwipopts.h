#ifndef LWIPOPTS_H_
#define LWIPOPTS_H_

// Memory options
#define MEM_ALIGNMENT                   4             // opt.h: 1
#define MEM_SIZE                        (5 * 1024)    // opt.h: 1600
#define MEMP_NUM_PBUF                   16            // opt.h: 16
#define MEMP_NUM_UDP_PCB                2             // opt.h: 4
#define MEMP_NUM_TCP_PCB                2             // opt.h: 5
#define MEMP_NUM_TCP_PCB_LISTEN         5             // opt.h: 8
#define MEMP_NUM_TCP_SEG                10            // opt.h: 16
#define MEMP_NUM_SYS_TIMEOUT            5             // opt.h: it depends

// Pbuf options
#define PBUF_POOL_SIZE                  12            // opt.h: 16
#define PBUF_POOL_BUFSIZE               512           // opt.h: it depends

// TCP options
#define LWIP_TCP                        1             // opt.h: 1

// ICMP options
#define LWIP_ICMP                       1             // opt.h: 1

// DHCP options
#define LWIP_DHCP                       0             // opt.h: 0

// UDP options
#define LWIP_UDP                        1             // opt.h: 1

// Statistics options
#define LWIP_STATS                      0             // opt.h: 1
#define LWIP_PROVIDE_ERRNO              1


// Checksum options
#define CHECKSUM_BY_HARDWARE            1

#if CHECKSUM_BY_HARDWARE
#define CHECKSUM_GEN_IP                 0             // opt.h: 1
#define CHECKSUM_GEN_UDP                0             // opt.h: 1
#define CHECKSUM_GEN_TCP                0             // opt.h: 1
#define CHECKSUM_GEN_ICMP               0             // opt.h: 1
#define CHECKSUM_CHECK_IP               0             // opt.h: 1
#define CHECKSUM_CHECK_UDP              0             // opt.h: 1
#define CHECKSUM_CHECK_TCP              0             // opt.h: 1
#define CHECKSUM_CHECK_ICMP             0             // opt.h: 1
#endif

// Sequential layer options
// LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
#define LWIP_NETCONN                    1             // opt.h: 1

// Socket options
// LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
#define LWIP_SOCKET                     0             // opt.h: 1

// OS options
#define TCPIP_THREAD_STACKSIZE          1000
#define TCPIP_MBOX_SIZE                 5
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_TCP_RECVMBOX_SIZE       2000
#define DEFAULT_ACCEPTMBOX_SIZE         2000
#define DEFAULT_THREAD_STACKSIZE        500
#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 2)

#endif /* LWIPOPTS_H_ */