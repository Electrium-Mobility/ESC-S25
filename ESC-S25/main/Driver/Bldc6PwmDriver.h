
#include "driver/mcpwm.h"
#include "math.h"
#include "util.h"
// #include "../Controller/BldcController.h" // Temporarily commented out to avoid include error

void pwm_timer_config(long pwm_freq, mcpwm_unit_t mcpwm_unit);


void pwm_config(long pwm_freq, int GPIO_A_H, int GPIO_A_L, int GPIO_B_H, int GPIO_B_L, int GPIO_C_H, int GPIO_C_L);

void pwm_write(float duty_A_H, float duty_A_L, float duty_B_H, float duty_B_L, float duty_C_H, float duty_C_L);

// void set_pwm(float Ua, float Ub, float Uc, float voltage_limit);
