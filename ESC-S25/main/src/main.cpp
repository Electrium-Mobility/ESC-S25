/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "main.h"
#include "PWM/BLDC6PWMmcpwm.h"
#include "motor/BLDCmotor_controller.h"


extern "C" void app_main(void)
{
    MCPWM_GPIO_init(GPIO_MCPWM0A_OUT, GPIO_MCPWM0B_OUT, GPIO_MCPWM1A_OUT, GPIO_MCPWM1B_OUT, GPIO_MCPWM2A_OUT, GPIO_MCPWM2B_OUT,
                    GPIO_MCPWM_CAP0_IN, GPIO_MCPWM_CAP1_IN, GPIO_MCPWM_CAP2_IN);
    MCPWM_config();

    
}