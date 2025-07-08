#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <math.h>
#include "esp_timer.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "driver/adc.h"

#include "util.h"

#define _PI 3.14159265359
#define _2_PI 6.28318530718
#define _3_PI 9.42477796077
#define _3_PI_2 4.71238898038
#define _PI_2 1.57079632679

// const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
TaskHandle_t handle_FOC = NULL;

TaskHandle_t handle_test_duty = NULL;
TaskHandle_t handle_pot_to_angle = NULL;

double shaft_angle = 0.0; // Mechanical angle of the motor shaft in radians

void pwm_timer_config(long pwm_freq, mcpwm_unit_t mcpwm_unit) {
    mcpwm_config_t pwm_config; // Initialize MCPWM configuration

    pwm_config.frequency = pwm_freq; // Set frequency of PWM signal
    pwm_config.counter_mode = MCPWM_UP_COUNTER; // Set counter mode
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0; // Set duty mode

    mcpwm_init(mcpwm_unit, MCPWM_TIMER_0, &pwm_config);
//     mcpwm_init(mcpwm_unit, MCPWM_TIMER_1, &pwm_config);
//     mcpwm_init(mcpwm_unit, MCPWM_TIMER_2, &pwm_config);

    // configure sync signals for timers
    mcpwm_sync_config_t sync_config = {
        .sync_sig = MCPWM_SELECT_TIMER0_SYNC, // Sync out signal with Timer 0
        .timer_val = 0, 
        .count_direction = MCPWM_TIMER_DIRECTION_UP, 
    };

    mcpwm_sync_configure(mcpwm_unit, MCPWM_TIMER_0, &sync_config);
//     mcpwm_sync_configure(mcpwm_unit, MCPWM_TIMER_1, &sync_config);
//     mcpwm_sync_configure(mcpwm_unit, MCPWM_TIMER_2, &sync_config);

    // Set all timers to be the TEZ (timer equals 0) of timer 0
    mcpwm_set_timer_sync_output(mcpwm_unit, MCPWM_TIMER_0, MCPWM_SWSYNC_SOURCE_TEZ); // Set Timer 0 as sync output
}

void pwm_config(long pwm_freq, int GPIO_A){
    mcpwm_pin_config_t pin_config = {
        .mcpwm0a_out_num = GPIO_A
    };
    mcpwm_set_pin(MCPWM_UNIT_0, &pin_config); // need to correct the hardcoded MCPWM_UNIT_0

    pwm_timer_config(pwm_freq, MCPWM_UNIT_0);
}


void pwm_write(float duty_A){
      mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, duty_A*100.0); 

      printf("Setting PWM duty cycle: %.2f\n", duty_A);
}

void set_pwm(float a, float voltage_limit) {
    float duty_A = constraint((a / _2_PI), 0, 1.0f);
    pwm_write(duty_A);
}

void test_duty(double shaft_angle) {
    float voltage_limit = 12.0f; // Example voltage limit
    set_pwm(shaft_angle, voltage_limit);
    printf("Shaft angle: %.2f, Voltage A: %.2f\n", shaft_angle, voltage_limit);
} 

double pot_to_angle(float pot_value) {
    // Map potentiometer value (0-4095) to angle (0-2π)
    return (pot_value / 4095.0f) * _2_PI;
}

void test_duty_task(void *pvParameters) {
    while (1) {
            test_duty(shaft_angle); // or use a variable/parameter as needed
            vTaskDelay(pdMS_TO_TICKS(100)); // adjust delay as needed
    }
}

void pot_to_angle_task(void *pvParameters) {
    while (1) {
            float pot_value = adc1_get_raw(ADC1_CHANNEL_0); // Read ADC value from channel 0
            // printf("Potentiometer value: %.2f\n", pot_value);
            shaft_angle = pot_to_angle(pot_value);
            // Do something with angle
            vTaskDelay(pdMS_TO_TICKS(100)); // adjust delay as needed
    }
}

void app_main(void)
{
      printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

      pwm_config(1000, 13);

      adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // Configure ADC1 channel 0 with 0 dB attenuation
      adc1_config_width(ADC_WIDTH_BIT_12); // Set ADC width to 12 bits

      xTaskCreate(test_duty_task, "TestDuty", 2048, NULL, 10, &handle_test_duty);
      xTaskCreate(pot_to_angle_task, "PotToAngle", 2048, NULL, 10, &handle_pot_to_angle);
}