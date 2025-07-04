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

#include "Controller/BldcController.h"
#include "Driver/Bldc6PwmDriver.h"


const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
TaskHandle_t handle_FOC = NULL;

void app_main(void)
{

      // adc_init();
      // gpio_init();
      // PWM_init();

      printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
      xTaskCreate(test_foc, "OPEN-LOOP", 2048, NULL, 10, &handle_FOC);

      while (1)
      {
            vTaskDelay(xDelay);
      }
}