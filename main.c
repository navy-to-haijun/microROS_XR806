/*
 * Copyright (C) 2017 XRADIO TECHNOLOGY CO., LTD. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the
 *       distribution.
 *    3. Neither the name of XRADIO TECHNOLOGY CO., LTD. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "common/framework/platform_init.h"
#include <stdio.h>
#include "kernel/os/os.h"
#include "driver/chip/hal_gpio.h"

#include "net/wlan/wlan.h"
#include "net/wlan/wlan_defs.h"
#include "common/framework/net_ctrl.h"
#include "common/framework/platform_init.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"


#define LED_PORT            GPIO_PORT_A
#define LED_PIN             GPIO_PIN_21

char *sta_ssid = "CU_7daysinn203";
char *sta_psk = "77777777";

#define REMOTE_IP	 "192.168.6.5"
#define PORT	     8080


HAL_Status status = HAL_ERROR;

extern void microros_pub_int32();
extern  void microros_pub_int32_init();

extern void microros_sub_int32();
extern void microros_sub_int32_init();

/*LED 初始化*/
static void led_init(void)
{
	GPIO_InitParam param;
	param.driving = GPIO_DRIVING_LEVEL_1;
	param.mode = GPIOx_Pn_F1_OUTPUT;
	param.pull = GPIO_PULL_NONE;
	HAL_GPIO_Init(LED_PORT, LED_PIN, &param);
}

uint8_t wlan_connected;
void wlan_ctrl_msg_proc(uint32_t event, uint32_t data, void *arg)
{
	uint16_t type = EVENT_SUBTYPE(event);

	switch (type) {
	case NET_CTRL_MSG_WLAN_CONNECTED:
		wlan_connected = 1;
		break;
	case NET_CTRL_MSG_WLAN_DISCONNECTED:
		wlan_connected = 0;
		break;
	case NET_CTRL_MSG_WLAN_SCAN_SUCCESS:
		break;
	case NET_CTRL_MSG_WLAN_SCAN_FAILED:
		break;
	case NET_CTRL_MSG_WLAN_4WAY_HANDSHAKE_FAILED:
		wlan_connected = 0;
		break;
	case NET_CTRL_MSG_WLAN_CONNECT_FAILED:
		wlan_connected = 0;
		break;
	case NET_CTRL_MSG_CONNECTION_LOSS:
		wlan_connected = 0;
		break;
	case NET_CTRL_MSG_NETWORK_UP:
		break;
	case NET_CTRL_MSG_NETWORK_DOWN:
		wlan_connected = 0;
		break;
	default:
		printf("unknown msg (%u, %u)\n", type, data);
		break;
	}
}

int wlan_net_ctrl_init(void)
{
	observer_base *ob = sys_callback_observer_create(CTRL_MSG_TYPE_NETWORK,
	                                                 NET_CTRL_MSG_ALL,
	                                                 wlan_ctrl_msg_proc,
	                                                 NULL);
	if (ob == NULL)
		return -1;
	if (sys_ctrl_attach(ob) != 0)
		return -1;

	return 0;
}

void wlan_connect(void)
{
	/* 查询网络状态 */
	wlan_net_ctrl_init();
	/* switch to sta mode */
	net_switch_mode(WLAN_MODE_STA);
	/* set ssid and password to wlan */
	wlan_sta_set((uint8_t *)sta_ssid, strlen(sta_ssid), (uint8_t *)sta_psk);
	/* start scan and connect to ap automatically */
	wlan_sta_enable();
	OS_Sleep(2);
	return;
}

/*UDP初始化*/
int sock;
struct sockaddr_in server_addr;

int16_t udpclient_init(){
	/*创建一个socket*/
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		printf("Socket error\n");
		return -1;
	}
	 /* 初始化预连接的服务端地址 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(REMOTE_IP);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
	/*连接*/
	if(connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("Connect <%d> fail!\n", sock);
        return -2;
	}
    printf("Connect %d sucessful!\n", sock);
  
	return 0;
}

char send_data[] = "send data to remote IP\n";
int16_t len;
char buffer[100];
int main(void)
{
	uint8_t led_flag = 0;
	platform_init();
	led_init();
	wlan_connect();
	HAL_GPIO_WritePin(LED_PORT, LED_PIN, 1);
	// microros_pub_int32_init();
	// microros_sub_int32_init();
	while (wlan_connected == 0) {
		printf("\n Wait for WiFi connection...\n\n");
		printf("\n or \n");
		printf("use this cmd to connect your ap with your own ssid and password:\n");
		printf("net sta config your_ssid your_password\n\n");
		wlan_connect();
	}
	OS_Sleep(2);

	if(udpclient_init() == 0){
		printf("Udp Init Successful");
	}
	/*发送测试*/
	len = sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	printf("len = %d", len);
	while (1) {
		if(wlan_connected){
			led_flag++;
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, led_flag % 2);
			/*接收测试*/
			len = recv(sock, (void *)buffer, 100, 0);
			if(len > 0){
				for(int16_t i = 0; i < len; i++)
				{
					printf("%c", buffer[i]);
				}
				printf("\n");
			}
			
			OS_MSleep(500);
		}
		/*WiFi中断*/
		else{
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, led_flag % 2);
			OS_MSleep(100);
		}
		

		// microros_pub_int32();
		// microros_sub_int32();

		
	}
	return 0;
}

