#include "BLDCDriver.h"
#include "BLDC6PWMmcpwm.h"
#include "DefaultValues.h"


class BLDC6PWMDriver : public BLDCDriver {
    public:

        /**
         * @brief Constructor for the BLDC6PWMDriver class
         * 
         * @param pin_A_PWM_H High PWM pin for phase A
         * @param pin_A_PWM_L Low PWM pin for phase A
         * @param pin_B_PWM_H High PWM pin for phase B
         * @param pin_B_PWM_L Low PWM pin for phase B
         * @param pin_C_PWM_H High PWM pin for phase C
         * @param pin_C_PWM_L Low PWM pin for phase C
         * @param enable_pin Optional enable pin for the driver, default is NOT_DEFINED
         */
        BLDC6PWMDriver(int pin_A_PWM_H, int pin_A_PWM_L, int pin_B_PWM_H, int pin_B_PWM_L, int pin_C_PWM_H, int pin_C_PWM_L, 
            int enable_pin = NOT_DEFINED);
        
        /**
         * @brief Constructor for the BLDC6PWMDriver class with voltage and frequency parameters
         */
        BLDC6PWMDriver(int pin_A_PWM_H, int pin_A_PWM_L, int pin_B_PWM_H, int pin_B_PWM_L, int pin_C_PWM_H, int pin_C_PWM_L, int enable_pin = NOT_DEFINED,
            float voltage_limit = NOT_DEFINED, float voltage_power_supply = DEFAULT_VOLTAGE_POWER_SUPPLY,
            int pwm_freq = NOT_DEFINED);
        
        
        void init(void) override;

        void enable(void) override;

        void disable(void) override;

        void setPwmVoltage(float Va, float Vb, float Vc) override;

        int pin_A_PWM_H; // High PWM pin for phase A
        int pin_A_PWM_L; // Low PWM pin for phase A
        int pin_B_PWM_H; // High PWM pin for phase B
        int pin_B_PWM_L; // Low PWM pin for phase B
        int pin_C_PWM_H; // High PWM pin for phase C
        int pin_C_PWM_L; // Low PWM pin for phase C
        int enable_pin; // Enable pin for the driver, if applicable
};
