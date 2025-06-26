#include "BLDC6PWMmcpwm.h"
#include <driver/mcpwm.h>
#include <driver/mcpwm_timer.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_attr.h"
#include "esp_timer.h"


#define TAG "MCPWM"


// This file only does the 6pwm
void MCPWM_GPIO_init(int GPIO_MCPWM0A, int GPIO_MCPWM0B, int GPIO_MCPWM1A, int GPIO_MCPWM1B, int GPIO_MCPWM2A, int GPIO_MCPWM2B,
                int GPIO_MCPWM_CAP0, int GPIO_MCPWM_CAP1, int GPIO_MCPWM_CAP2){
                    
    // Initialize MCPWM GPIOs
    ESP_LOGI(TAG, "Initializing MCPWM GPIOs...");         

    // Initialize MCPWM gpio
    // //MCPWM_Unit_0 -> First MCPWM unit is used for the first three phases
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_MCPWM0A); // Set GPIO as PWM0A
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0B, GPIO_MCPWM0B); // Set GPIO as PWM0B
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, GPIO_MCPWM1A); // Set GPIO as PWM1A
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, GPIO_MCPWM1B); // Set GPIO as PWM1B
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, GPIO_MCPWM2A); // Set GPIO as PWM2A
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2B, GPIO_MCPWM2B); // Set GPIO as PWM2B

    mcpwm_pin_config_t pin_config = {
        .mcpwm0a_out_num = GPIO_MCPWM0A,
        .mcpwm0b_out_num = GPIO_MCPWM0B,
        .mcpwm1a_out_num = GPIO_MCPWM1A,
        .mcpwm1b_out_num = GPIO_MCPWM1B,
        .mcpwm2a_out_num = GPIO_MCPWM2A,
        .mcpwm2b_out_num = GPIO_MCPWM2B,
    };
    mcpwm_set_pin(MCPWM_UNIT_0, &pin_config);

    // // MCPWM_Unit_1 -> Second MCPWM unit is used for capture
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_0, GPIO_MCPWM_CAP0); // Set GPIO as CAP0
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_1, GPIO_MCPWM_CAP1); // Set GPIO as CAP1
    // mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM_CAP_2, GPIO_MCPWM_CAP2); // Set GPIO as CAP2

    mcpwm_pin_config_t pin_config_capture = {
        .mcpwm_cap0_in_num = GPIO_MCPWM_CAP0,
        .mcpwm_cap1_in_num = GPIO_MCPWM_CAP1,
        .mcpwm_cap2_in_num = GPIO_MCPWM_CAP2,
    };


    ESP_LOGI(TAG, "MCPWM GPIO initialized");

    // Configure MCPWM
    MCPWM_config();

    // Start MCPWM
    MCPWM_start();
}

void MCPWM_config() {
    ESP_LOGI(TAG, "Configuring MCPWM...");

    // MCPWM configuration
    mcpwm_config_t pwm_config;
    pwm_config.frequency = MCPWM_FREQ; // Set frequency
    pwm_config.cmpr_a = MCPWM_DUTYCYCLE_A; // Set duty cycle for PWM0A
    pwm_config.cmpr_b = MCPWM_DUTYCYCLE_B; // Set duty cycle for PWM0B
    pwm_config.counter_mode = MCPWM_UP_COUNTER; // Set counter mode
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0; // Set duty mode, MCPWM_DUTY_MODE_0 -> Active high duty, MCPWM_DUTY_MODE_1 -> Active low duty

    // Initialize MCPWM unit 0 and timer 0
    // notice uses legacy API -> 4.3
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);

    ESP_LOGI(TAG, "MCPWM configured");

}



void MCPWM_start(){
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_1);
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_2);
}

// COMPLETED





