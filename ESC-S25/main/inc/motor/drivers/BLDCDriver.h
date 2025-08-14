#pragma once

#include "esp_log.h"

class BLDCDriver {
    public:
        virtual ~BLDCDriver() = default;

        virtual void init(void) = 0;

        virtual void enable(void) = 0;

        virtual void disable(void) = 0;

        int pwm_freq;
        float voltage_limit;
        float voltage_power_supply;

        /*
        * @param Va - voltage of phase A
        * @param Vb - voltage of phase B
        * @param Vc - voltage of phase C
        * @note Va, Vb, Vc is NA notation, in EU they have Ua, Ub, Uc
        */
        virtual void setPwmVoltage(float Va, float Vb, float Vc) = 0;
};
