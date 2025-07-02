
#include "driver/mcpwm.h"


void pwm_timer_config(long pwm_freq, float dead_time, mcpwm_unit_t mcpwm_unit);


void pwm_config(long pwm_freq, float dead_time, int GPIO_A_H, int GPIO_A_L, int GPIO_B_H, int GPIO_B_L, int GPIO_C_H, int GPIO_C_L);


void pwm_write(float duty_A, float duty_B, float duty_C, int GPIO_A_H, int GPIO_A_L, int GPIO_B_H, int GPIO_B_L, int GPIO_C_H, int GPIO_C_L);
