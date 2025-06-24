#include "BLDC6PWMDriver.h"
#include "DefaultValues.h"


BLDC6PWMDriver::BLDC6PWMDriver(int pin_A_PWM_H, int pin_A_PWM_L, int pin_B_PWM_H, int pin_B_PWM_L, int pin_C_PWM_H, int pin_C_PWM_L, int enable_pin) {
    this->pin_A_PWM_H = pin_A_PWM_H;
    this->pin_A_PWM_L = pin_A_PWM_L;
    this->pin_B_PWM_H = pin_B_PWM_H;
    this->pin_B_PWM_L = pin_B_PWM_L;
    this->pin_C_PWM_H = pin_C_PWM_H;
    this->pin_C_PWM_L = pin_C_PWM_L;
    this->enable_pin = enable_pin;

    voltage_limit = NOT_DEFINED;
    voltage_power_supply = DEFAULT_VOLTAGE_POWER_SUPPLY;
    pwm_freq = NOT_DEFINED;
}

/**
 * @brief Overload constructor for the BLDC6PWMDriver class with voltage and frequency parameters
 */
BLDC6PWMDriver::BLDC6PWMDriver(int pin_A_PWM_H, int pin_A_PWM_L, int pin_B_PWM_H, int pin_B_PWM_L, int pin_C_PWM_H, int pin_C_PWM_L, int enable_pin,
                               float voltage_limit, float voltage_power_supply, int pwm_freq) {
    this->pin_A_PWM_H = pin_A_PWM_H;
    this->pin_A_PWM_L = pin_A_PWM_L;
    this->pin_B_PWM_H = pin_B_PWM_H;
    this->pin_B_PWM_L = pin_B_PWM_L;
    this->pin_C_PWM_H = pin_C_PWM_H;
    this->pin_C_PWM_L = pin_C_PWM_L;
    this->enable_pin = enable_pin;

    this->voltage_limit = voltage_limit;
    this->voltage_power_supply = voltage_power_supply;
    this->pwm_freq = pwm_freq;
}


void BLDC6PWMDriver::init(void) {
    ESP_LOGI(TAG, "Initializing BLDC6PWMDriver...");

    // Initialize GPIOs for MCPWM
    MCPWM_GPIO_init(pin_A_PWM_H, pin_A_PWM_L, pin_B_PWM_H, pin_B_PWM_L, pin_C_PWM_H, pin_C_PWM_L,
                    GPIO_MCPWM_CAP0_IN, GPIO_MCPWM_CAP1_IN, GPIO_MCPWM_CAP2_IN);

    // Configure MCPWM
    MCPWM_config();

    ESP_LOGI(TAG, "BLDC6PWMDriver initialized");
}



