#include "BLDC6PWMmcpwm.h"
// Legacy MCPWM driver (cannot be mixed with the driver/mcpwm_prelude.h API)
#include <driver/mcpwm.h>
#include "esp_log.h"

#define TAG "MCPWM"

// This file only does the 6pwm
void MCPWM_GPIO_init(int GPIO_MCPWM0A, int GPIO_MCPWM0B, int GPIO_MCPWM1A, int GPIO_MCPWM1B, int GPIO_MCPWM2A, int GPIO_MCPWM2B,
                int GPIO_MCPWM_CAP0, int GPIO_MCPWM_CAP1, int GPIO_MCPWM_CAP2){

    ESP_LOGI(TAG, "Initializing MCPWM GPIOs...");

    // Unused signals must be -1, otherwise they default to GPIO 0
    mcpwm_pin_config_t pin_config = {
        .mcpwm0a_out_num = GPIO_MCPWM0A,
        .mcpwm0b_out_num = GPIO_MCPWM0B,
        .mcpwm1a_out_num = GPIO_MCPWM1A,
        .mcpwm1b_out_num = GPIO_MCPWM1B,
        .mcpwm2a_out_num = GPIO_MCPWM2A,
        .mcpwm2b_out_num = GPIO_MCPWM2B,
        .mcpwm_sync0_in_num = -1,
        .mcpwm_sync1_in_num = -1,
        .mcpwm_sync2_in_num = -1,
        .mcpwm_fault0_in_num = -1,
        .mcpwm_fault1_in_num = -1,
        .mcpwm_fault2_in_num = -1,
        .mcpwm_cap0_in_num = GPIO_MCPWM_CAP0,
        .mcpwm_cap1_in_num = GPIO_MCPWM_CAP1,
        .mcpwm_cap2_in_num = GPIO_MCPWM_CAP2,
    };
    ESP_ERROR_CHECK(mcpwm_set_pin(MCPWM_UNIT_0, &pin_config));

    ESP_LOGI(TAG, "MCPWM GPIO initialized");
}

void MCPWM_config() {
    ESP_LOGI(TAG, "Configuring MCPWM...");

    mcpwm_config_t pwm_config = {};
    pwm_config.frequency = MCPWM_FREQ;
    pwm_config.cmpr_a = MCPWM_DUTYCYCLE_A;
    pwm_config.cmpr_b = MCPWM_DUTYCYCLE_B;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0; // MCPWM_DUTY_MODE_0 -> Active high duty, MCPWM_DUTY_MODE_1 -> Active low duty

    const mcpwm_timer_t timers[] = {MCPWM_TIMER_0, MCPWM_TIMER_1, MCPWM_TIMER_2};
    for (mcpwm_timer_t timer : timers) {
        ESP_ERROR_CHECK(mcpwm_init(MCPWM_UNIT_0, timer, &pwm_config));
#if MCPWM_EN_DEADTIME
        // B output is the complement of A, with dead time on both edges
        ESP_ERROR_CHECK(mcpwm_deadtime_enable(MCPWM_UNIT_0, timer, MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE,
                                              MCPWM_DEADTIME_TICKS, MCPWM_DEADTIME_TICKS));
#endif
    }

    ESP_LOGI(TAG, "MCPWM configured");
}

void MCPWM_start() {
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_1);
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_2);
}

void MCPWM_stop() {
    mcpwm_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
    mcpwm_stop(MCPWM_UNIT_0, MCPWM_TIMER_1);
    mcpwm_stop(MCPWM_UNIT_0, MCPWM_TIMER_2);
}

void MCPWM_set_duty(float duty_a, float duty_b, float duty_c) {
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_a);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, duty_b);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, duty_c);
}
