#include "esp_log.h"

class BLDCDriver{
    public:

        virtual void init(void);

        virtual void enable(void);

        virtual void disable(void);

        int pwm_freq;
        float voltage_limit;
        float voltage_power_supply;


        /*
        * @param Va - voltage of phase A
        * @param Vb - voltage of phase B
        * @param Vc - voltage of phase C
        * @note Va, Vb, Vc is NA notation, in EU they have Ua, Ub, Uc
        */
        virtual void setPwmVoltage(float Va, float Vb, float Vc);
};