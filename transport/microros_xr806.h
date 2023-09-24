#ifndef MICRO_ROS_XR806
#define MICRO_ROS_RTT

#include <stdio.h>
#include "kernel/os/os.h"
#include "driver/chip/hal_uart.h"

#include <uxr/client/transport.h>
#include <rmw_microros/rmw_microros.h>


#define MICROROS_SERIAL

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

#endif


