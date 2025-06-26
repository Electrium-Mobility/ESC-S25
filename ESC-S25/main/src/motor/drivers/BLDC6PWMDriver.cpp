#include "BLDC6PWMDriver.h"
#include "DefaultValues.h"
#include "esp_log.h"
#include "GPIO.h"

#define TAG "BLDC6PWMDriver"

BLDC6PWMDriver::BLDC6PWMDriver(int pin_A_PWM_H, int pin_A_PWM_L, int pin_B_PWM_H, int pin_B_PWM_L, int pin_C_PWM_H, int pin_C_PWM_L, 
        int pin_CAP0, int pin_CAP1, int pin_CAP2, int enable_pin) {
    this->pin_A_PWM_H = pin_A_PWM_H;
    this->pin_A_PWM_L = pin_A_PWM_L;
    this->pin_B_PWM_H = pin_B_PWM_H;
    this->pin_B_PWM_L = pin_B_PWM_L;
    this->pin_C_PWM_H = pin_C_PWM_H;
    this->pin_C_PWM_L = pin_C_PWM_L;
    this->pin_CAP0 = pin_CAP0;
    this->pin_CAP1 = pin_CAP1;
    this->pin_CAP2 = pin_CAP2;
    this->enable_pin = enable_pin;

    voltage_limit = NOT_DEFINED;
    voltage_power_supply = DEFAULT_VOLTAGE_POWER_SUPPLY;
    pwm_freq = NOT_DEFINED;
}

/**
 * @brief Overload constructor for the BLDC6PWMDriver class with voltage and frequency parameters
 */
BLDC6PWMDriver::BLDC6PWMDriver(int pin_A_PWM_H, int pin_A_PWM_L, int pin_B_PWM_H, int pin_B_PWM_L, int pin_C_PWM_H, int pin_C_PWM_L, 
        int pin_CAP0, int pin_CAP1, int pin_CAP2, int enable_pin = NOT_DEFINED,
        float voltage_limit = NOT_DEFINED, float voltage_power_supply = DEFAULT_VOLTAGE_POWER_SUPPLY,
        int pwm_freq = NOT_DEFINED) {
    this->pin_A_PWM_H = pin_A_PWM_H;
    this->pin_A_PWM_L = pin_A_PWM_L;
    this->pin_B_PWM_H = pin_B_PWM_H;
    this->pin_B_PWM_L = pin_B_PWM_L;
    this->pin_C_PWM_H = pin_C_PWM_H;
    this->pin_C_PWM_L = pin_C_PWM_L;
    this->pin_CAP0 = pin_CAP0;
    this->pin_CAP1 = pin_CAP1;
    this->pin_CAP2 = pin_CAP2;
    this->enable_pin = enable_pin;

    this->voltage_limit = voltage_limit;
    this->voltage_power_supply = voltage_power_supply;
    this->pwm_freq = pwm_freq;
}


void BLDC6PWMDriver::Init(void) {
    ESP_LOGI(TAG, "Initializing BLDC6PWMDriver...");

    // Initialize GPIOs for MCPWM
    MCPWM_GPIO_init(pin_A_PWM_H, pin_A_PWM_L, pin_B_PWM_H, pin_B_PWM_L, pin_C_PWM_H, pin_C_PWM_L,
                    pin_CAP0, pin_CAP1, pin_CAP2);

    // Configure MCPWM
    MCPWM_config();

    ESP_LOGI(TAG, "BLDC6PWMDriver initialized");
}

void BLDC6PWMDriver::Enable(void) {
    ESP_LOGI(TAG, "Enabling BLDC6PWMDriver...");

    // if (enable_pin != NOT_DEFINED) {
    //     gpio_set_level(enable_pin, 1); // Set enable pin high
    // }

    ESP_LOGI(TAG, "BLDC6PWMDriver enabled");
}

void BLDC6PWMDriver::Disable(void) {
    ESP_LOGI(TAG, "Disabling BLDC6PWMDriver...");

    // if (enable_pin != NOT_DEFINED) {
    //     gpio_set_level(enable_pin, 0); // Set enable pin low
    // }

    ESP_LOGI(TAG, "BLDC6PWMDriver disabled");
}

void BLDC6PWMDriver::SetPwmVoltage(float Va, float Vb, float Vc) {
    ESP_LOGI(TAG, "Setting PWM voltages: Va=%f, Vb=%f, Vc=%f", Va, Vb, Vc);

    float duty_cycle_A = Va / voltage_power_supply;
    float duty_cycle_B = Vb / voltage_power_supply;
    float duty_cycle_C = Vc / voltage_power_supply;

    // Set the PWM duty cycles for each phase
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_cycle_A * 100.0);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, Vb / voltage_power_supply * 100.0);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, Vc / voltage_power_supply * 100.0);
}


