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
#include "sdkconfig.h"
#include "esp_log.h"

#include "Controller/BldcController.h"
#include "Driver/Bldc6PwmDriver.h"

#include "driver/adc.h"

const TickType_t xDelay = 50 / portTICK_PERIOD_MS;

TaskHandle_t handle_test_duty = NULL;
TaskHandle_t handle_pot_to_angle = NULL;


double pot_to_angle(float pot_value)
{
    // Map potentiometer value (0-4095) to angle (0-2π)
    return (pot_value / 4095.0f) * _2_PI;
}

void pot_to_angle_task(void *pvParameters)
{
    while(1){
        int raw = adc1_get_raw(ADC1_CHANNEL_0);
        if (raw < 0) {
            printf("ADC read error!\n");
        } else {
            pot_angle = pot_to_angle((float)raw);
        }
        
        vTaskDelay(xDelay); // Delay for 100 milliseconds
    }


    // vTaskDelete(NULL); // Cleanly delete the task before returning
}

void foc_task(void *pvParameters) {

    while(1){
        float target_velocity = (pot_angle / _2_PI) * 10; // Scale pot value to target velocity

        velocity_open_loop(target_velocity);

        // vTaskDelay(pdMS_TO_TICKS(100));
        // vTaskDelete(NULL); // Cleanly delete the task before returning
        vTaskDelay(xDelay); // Delay for 100 milliseconds
    }
   
}

void app_main(void)
{

    // adc_init();
    // gpio_init();
    // PWM_init();
    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12); // Configure ADC1 channel 0 with 12 dB attenuation
    adc1_config_width(ADC_WIDTH_BIT_12);              // Set ADC width to 12 bits

    foc_init();
    
    // pot_angle = pot_to_angle(1000);
    // xTaskCreate(pot_to_angle_task, "PotToAngle", 2048, NULL, 10, &handle_pot_to_angle);
    xTaskCreatePinnedToCore(pot_to_angle_task, "PotToAngle", 2048, NULL, 10, &handle_pot_to_angle, 1); // Create the pot to angle task on core 1
    xTaskCreatePinnedToCore(foc_task, "TestDuty", 4096, NULL, 10, &handle_test_duty, 1); // Create the FOC task on core 1

    // xTaskCreate(test_foc, "TestDuty", 4096, NULL, 10, &handle_test_duty);
}