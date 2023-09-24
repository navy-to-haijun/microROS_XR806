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

#define LED_PORT            GPIO_PORT_A
#define LED_PIN             GPIO_PIN_21


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


int main(void)
{
	uint8_t led_flag = 0;
	platform_init();
	led_init();
	// microros_pub_int32_init();
	microros_sub_int32_init();

	while (1) {
		led_flag++;
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, led_flag % 2);

		// microros_pub_int32();
		microros_sub_int32();

		// OS_MSleep(500);
	}
	return 0;
}

