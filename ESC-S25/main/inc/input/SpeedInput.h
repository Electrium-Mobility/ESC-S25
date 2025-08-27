#pragma once

/// Throttle / potentiometer input on an ADC1 pin.
class SpeedInput {
    public:
        /// pin must be an ADC1 GPIO (32..39 on the ESP32). deadband is the fraction
        /// of travel treated as zero, so a noisy pot at rest doesn't spin the motor.
        SpeedInput(int pin, float deadband = 0.03f);

        void init(void);

        /// Filtered throttle position in [0, 1] (0 if the ADC read failed).
        float read(void);

    private:
        int pin;
        float deadband;
        float filtered = 0.0f;
        void *handle = nullptr; // adc_oneshot_unit_handle_t
        int channel = 0;
};
