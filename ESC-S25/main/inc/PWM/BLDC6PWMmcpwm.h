#pragma once

#include "freertos/FreeRTOS.h"
#include "esp_attr.h"
#include "esp_timer.h"
#include "esp_log.h"

#define MCPWM_EN_DEADTIME 1 // Enable dead time insertion
#define MCPWM_EN_SYNC 1 // Enable synchronization between timers

#define MCPWM_FREQ 40000 // PWM frequency in Hz
#define MCPWM_DUTYCYCLE_A 50 // Duty cycle for phase A in percentage
#define MCPWM_DUTYCYCLE_B 50 // Duty cycle for phase B in percentage

#define MCPWM_DEADTIME_TICKS 20 // Dead time in 100 ns ticks (20 -> 2 us)

void MCPWM_GPIO_init(int GPIO_MCPWM0A, int GPIO_MCPWM0B, int GPIO_MCPWM1A, int GPIO_MCPWM1B, int GPIO_MCPWM2A, int GPIO_MCPWM2B,
                int GPIO_MCPWM_CAP0, int GPIO_MCPWM_CAP1, int GPIO_MCPWM_CAP2);

void MCPWM_config();

void MCPWM_start();

void MCPWM_stop();

/**
 * @brief Set the high-side duty cycle of each phase (the low side is complementary)
 * @param duty_a Duty cycle of phase A in percent [0, 100]
 * @param duty_b Duty cycle of phase B in percent [0, 100]
 * @param duty_c Duty cycle of phase C in percent [0, 100]
 */
void MCPWM_set_duty(float duty_a, float duty_b, float duty_c);
