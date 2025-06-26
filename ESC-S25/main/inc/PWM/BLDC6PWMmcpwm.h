#include "freertos/FreeRTOS.h"
#include "esp_attr.h"
#include "esp_timer.h"
#include "driver/mcpwm_prelude.h"
#include "driver/mcpwm_timer.h"
#include "esp_log.h"

#define PWM_CLK_PRESCALER // Prescaler for the PWM clock between 0 and 255


#define MCPWM_EN_DEADTIME 1 // Enable dead time insertion
#define MCPWM_EN_SYNC 1 // Enable synchronization between timers

#define MCPWM_FREQ 40000 // default frequency 20khz - centered pwm has twice lower frequency
#define MCPWM_DUTYCYCLE_A 50 // Duty cycle for phase A in percentage
#define MCPWM_DUTYCYCLE_B 50 // Duty

#define DEFAULT_DEAD_TIME 0.2 // default dead time is 2%


void MCPWM_GPIO_init(int GPIO_MCPWM0A, int GPIO_MCPWM0B, int GPIO_MCPWM1A, int GPIO_MCPWM1B, int GPIO_MCPWM2A, int GPIO_MCPWM2B,
                int GPIO_MCPWM_CAP0, int GPIO_MCPWM_CAP1, int GPIO_MCPWM_CAP2);

void MCPWM_config();
