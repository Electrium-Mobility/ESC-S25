#include "BLDC6PWMDriver.h"
#include "DefaultValues.h"
#include "esp_log.h"
#include "driver/gpio.h"

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
        int pin_CAP0, int pin_CAP1, int pin_CAP2, int enable_pin,
        float voltage_limit, float voltage_power_supply, int pwm_freq) {
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


void BLDC6PWMDriver::init(void) {
    ESP_LOGI(TAG, "Initializing BLDC6PWMDriver...");

    // Initialize GPIOs for MCPWM
    MCPWM_GPIO_init(pin_A_PWM_H, pin_A_PWM_L, pin_B_PWM_H, pin_B_PWM_L, pin_C_PWM_H, pin_C_PWM_L,
                    pin_CAP0, pin_CAP1, pin_CAP2);

    // Configure MCPWM
    MCPWM_config();

    // Optional gate driver enable line, held low (driver off) until enable()
    if (enable_pin != NOT_DEFINED) {
        gpio_reset_pin((gpio_num_t)enable_pin);
        gpio_set_direction((gpio_num_t)enable_pin, GPIO_MODE_OUTPUT);
        gpio_set_level((gpio_num_t)enable_pin, 0);
    }

    if (voltage_limit == NOT_DEFINED || voltage_limit > voltage_power_supply) {
        voltage_limit = voltage_power_supply;
    }

    ESP_LOGI(TAG, "BLDC6PWMDriver initialized");
}

void BLDC6PWMDriver::enable(void) {
    MCPWM_start();
    if (enable_pin != NOT_DEFINED) {
        gpio_set_level((gpio_num_t)enable_pin, 1);
    }
}

void BLDC6PWMDriver::disable(void) {
    // Drop the gate driver first so stopping the timers can't leave a leg shorted
    if (enable_pin != NOT_DEFINED) {
        gpio_set_level((gpio_num_t)enable_pin, 0);
    }
    MCPWM_stop();
}

void BLDC6PWMDriver::setPwmVoltage(float Va, float Vb, float Vc) {
    // Convert phase voltages to duty cycles (percent) of the supply voltage
    auto to_duty = [this](float v) {
        if (v < 0.0f) v = 0.0f;
        if (v > voltage_limit) v = voltage_limit;
        return 100.0f * v / voltage_power_supply;
    };
    MCPWM_set_duty(to_duty(Va), to_duty(Vb), to_duty(Vc));
}
