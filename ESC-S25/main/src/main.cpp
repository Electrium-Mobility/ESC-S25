/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "main.h"
#include "motor/BLDC6PWMmotor_controller.h"
#include "motor/drivers/BLDC6PWMDriver.h"

extern "C" void app_main(void)
{
    // init() sets up the GPIOs and configures the MCPWM timers
    static BLDC6PWMDriver driver(GPIO_MCPWM0A_OUT, GPIO_MCPWM0B_OUT, GPIO_MCPWM1A_OUT, GPIO_MCPWM1B_OUT,
                                 GPIO_MCPWM2A_OUT, GPIO_MCPWM2B_OUT,
                                 GPIO_MCPWM_CAP0_IN, GPIO_MCPWM_CAP1_IN, GPIO_MCPWM_CAP2_IN);
    driver.init();
    driver.enable();
    AttachDriver(&driver);

    // Open-loop demo: step the electrical angle around one revolution
    double angle = 0;
    while (true) {
        SetPhaseVoltage(2.0, 0.0, angle);
        angle = NormalizeAngle(angle + PI / 180.0);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
