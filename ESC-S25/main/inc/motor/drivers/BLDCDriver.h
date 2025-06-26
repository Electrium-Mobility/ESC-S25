#include "esp_log.h"

class BLDCDriver{
    public:

        virtual void Init(void);

        virtual void Enable(void);

        virtual void Disable(void);

        int pwm_freq;
        float voltage_limit;
        float voltage_power_supply;


        /*
        * @param Va - voltage of phase A
        * @param Vb - voltage of phase B
        * @param Vc - voltage of phase C
        * @note Va, Vb, Vc is NA notation, in EU they have Ua, Ub, Uc
        */
        virtual void SetPwmVoltage(float Va, float Vb, float Vc);
};