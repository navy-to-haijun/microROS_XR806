#ifndef MICRO_ROS_XR806
#define MICRO_ROS_RTT

#include <stdio.h>
#include "kernel/os/os.h"
#include "driver/chip/hal_uart.h"
#include "lwip/sockets.h"

#include <sys/time.h>

#include <uxr/client/transport.h>
#include <rmw_microros/rmw_microros.h>

/*microROS相关宏定义*/
/*通信方式*/
#define MICROROS_UDP 
// #define MICROROS_SERIAL

/*demo选择*/
// #define MICROROS_PUB_INT32
// #define MICROROS_SUB_INT32
#define MICROROS_PUB_SUB



#if defined MICROROS_SERIAL

extern bool serial_transport_open(uxrCustomTransport* transport);
extern bool serial_transport_close(uxrCustomTransport* transport);
extern size_t serial_transport_write(uxrCustomTransport* transport,const uint8_t* buffer,size_t length,uint8_t* errcode);
extern size_t serial_transport_read(uxrCustomTransport* transport,uint8_t* buffer,size_t length,int timeout,uint8_t* errcode);
extern int clock_gettime(clockid_t unused, struct timespec *tp);


static inline void  set_microros_transports()
{
    rmw_uros_set_custom_transport(
                1,
                (void *) NULL,
                serial_transport_open,
                serial_transport_close,
                serial_transport_write,
                serial_transport_read
            );
}
#endif

#if defined MICROROS_UDP

extern bool udp_transport_open(uxrCustomTransport* transport);
extern bool udp_transport_close(uxrCustomTransport* transport);
extern size_t udp_transport_write(uxrCustomTransport* transport,const uint8_t* buffer,size_t length,uint8_t* errcode);
extern size_t udp_transport_read(uxrCustomTransport* transport,uint8_t* buffer,size_t length,int timeout,uint8_t* errcode);
extern int clock_gettime(clockid_t unused, struct timespec *tp);


static inline void  set_microros_transports()
{
    rmw_uros_set_custom_transport(
                false,
                (void *) NULL,
                udp_transport_open,
                udp_transport_close,
                udp_transport_write,
                udp_transport_read
            );
}
#endif


#endif


