# include"transport/microros_xr806.h"

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>

#include <stdio.h>

#if defined MICROROS_SUB_INT32

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); return;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

static rcl_subscription_t subscriber;
static std_msgs__msg__Int32 recv_msg;
static rclc_executor_t executor;
static rclc_support_t support;
static rcl_allocator_t allocator;
static rcl_node_t node;

static int count= 0;
static void subscription_callback(const void * msgin)
{
    count ++;
	const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msgin;
	printf("%d: Received: %d\n", count, msg->data);
}

void microros_sub_int32()
{
    OS_MSleep(100);
    RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));

}

void microros_sub_int32_init()
{
    set_microros_transports();

    allocator = rcl_get_default_allocator();
    // create init_options
	RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
    // create node
	RCCHECK(rclc_node_init_default(&node, "xr806_node", "", &support));

	// create subscriber
	RCCHECK(rclc_subscription_init_default(
		&subscriber,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
		"xr806_node_subscriber"));
    
    // create executor
	RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
	RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &recv_msg, &subscription_callback, ON_NEW_DATA));

     printf("micro_ros init successful.\n");
    // ros2 topic pub /micro_ros_xr806_node_subscriber std_msgs/msg/Int32 data:\ 12
}
#endif