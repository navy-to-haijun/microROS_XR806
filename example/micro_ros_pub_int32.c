
# include"transport/microros_xr806.h"

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

#if defined MICROROS_PUB_INT32

static rcl_publisher_t publisher;
static std_msgs__msg__Int32 msg;

static rclc_executor_t executor;
static rclc_support_t support;
static rcl_allocator_t allocator;

static rcl_node_t node;
static rcl_timer_t timer;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); return;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

static void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{  
    if (timer != NULL) 
    {
        RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
        msg.data++;
         printf("msg.data = %d\n",  msg.data);
    }
    else {
        printf("micrros timer null\n");
    }
}

void microros_pub_int32()
{
    OS_MSleep(100);
    RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));

}

void microros_pub_int32_init()
{
    set_microros_transports();
    allocator = rcl_get_default_allocator();

    //create init_options
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
    // create node
    RCCHECK(rclc_node_init_default(&node, "xr806_node", "", &support));
    // create publisher
    RCCHECK(rclc_publisher_init_default(
      &publisher,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      "xr806_node_publisher")
      );
    // create timer
    const unsigned int timer_timeout = 1000;
    RCCHECK(rclc_timer_init_default(
      &timer,
      &support,
      RCL_MS_TO_NS(timer_timeout),
      timer_callback)
      );
    // create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
    RCCHECK(rclc_executor_add_timer(&executor, &timer));

    msg.data = 0;
    printf("micro_ros init successful.\n");
}
#endif

