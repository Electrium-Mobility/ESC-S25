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

const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
TaskHandle_t handle_FOC = NULL;

TaskHandle_t ADCTaskHandle = NULL;

int adc_value = 0;

void app_main(void)
{

      // adc_init();
      // gpio_init();
      // PWM_init();
      printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
      xTaskCreate(test_foc, "OPEN-LOOP", 2048, NULL, 10, &handle_FOC);

      adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // Configure ADC1 channel 0 with 0 dB attenuation
      adc1_config_width(ADC_WIDTH_BIT_12); // Set ADC width to 12 bits

      while (1)
      {     
            adc_value = adc1_get_raw(ADC1_CHANNEL_0); // Read ADC value from channel 0
            printf("ADC Value: %d\n", adc_value); // Print the ADC value


            vTaskDelay(xDelay);
      }
}