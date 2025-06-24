// #include "BLDCDriver.h"
// #include "DefaultValues.h"


// class BLDC3PWMDriver : public BLDCDriver {
//     public:

//         /**
//          * @brief Constructor for the BLDC3PWMDriver class
//          * 
//          * @param pin_A_PWM PWM pin for phase A
//          * @param pin_B_PWM PWM pin for phase B
//          * @param pin_C_PWM PWM pin for phase C
//          * @param enable_pin Optional enable pin for the driver, default is NOT_DEFINED
//          */
//         BLDC3PWMDriver(int pin_A_PWM, int pin_B_PWM, int pin_C_PWM, 
//             int enable_pin = NOT_DEFINED);
        
//         /**
//          * @brief Constructor for the BLDC3PWMDriver class with voltage and frequency parameters
//          */
//         BLDC3PWMDriver(int pin_A_PWM, int pin_B_PWM, int pin_C_PWM, int enable_pin = NOT_DEFINED,
//             float voltage_limit = NOT_DEFINED, float voltage_power_supply = DEFAULT_VOLTAGE_POWER_SUPPLY,
//             int pwm_freq = NOT_DEFINED);
        
        
//         void init(void) override;

//         void enable(void) override;

//         void disable(void) override;

//         void setPwmVoltage(float Va, float Vb, float Vc) override;

//         int pin_A_PWM; // PWM pin for phase A
//         int pin_B_PWM; // PWM pin for phase B
//         int pin_C_PWM; // PWM pin for phase C
//         int enable_pin; // Enable pin for the driver, if applicable
// };



// // class motor_controller {
// //     public:
// //         motor_controller(int pin_A_PWM, int pin_B_PWM, int pin_C_PWM, float velocity_limit, float voltage_limit, float current_limit, FOC_MODULATION modulation)
// //         {
// //             this->pin_A_PWM = pin_A_PWM;
// //             this->pin_B_PWM = pin_B_PWM;
// //             this->pin_C_PWM = pin_C_PWM;
// //             this->velocity_limit = velocity_limit;
// //             this->voltage_limit = voltage_limit;
// //             this->current_limit = current_limit;
// //             this->modulation = modulation;
// //         }
        

// //     private:
// //         int pin_A_PWM; // PWM pin for phase A
// //         int pin_B_PWM; // PWM pin for phase B
// //         int pin_C_PWM; // PWM pin for phase C

// //         float velocity_limit; // angular velocity limit in rad/s

// //         float voltage_limit;

// //         float current_limit; // current limit in A

// //         FOC_MODULATION modulation; // FOC modulation type
    

// // };