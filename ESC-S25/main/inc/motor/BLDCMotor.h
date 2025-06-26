//         FOC_MODULATION modulation; // default modulation is trapezoidal 120
#include "math.h"
#include "BLDC6PWMmcpwm.h"
#include "BLDC6PWMDriver.h"


#define PI 3.14159265358979323846


typedef enum
{
    Trapezoid_120 = 0,
    Sinusoidal = 1,
    Space_Vector = 2,
} FOC_MODULATION;


typedef enum
{
    Clk_CW = 0,
    Clk_CCW = 1,
} Direction;

class BLDCMotor {
    public:

        BLDCMotor(int pole_pairs);

        BLDCDriver *driver; // Pointer to the driver

        /*
        * @brief Connect the driver to the motor controller
        */
        virtual void DriverInit(BLDC6PWMDriver *driver);

        /*
        * @brief Initialize the motor controller
        */
        virtual void ControllerInit(Direction motor_direction = Clk_CW);

        virtual void Update(void);

        virtual void MoveTo(float angle);

        virtual void SetPwmFreq(int pwm_freq);
    
    protected:
        int pole_pairs; // Number of pole pairs in the motor

        FOC_MODULATION modulation = Trapezoid_120; // default modulation is trapezoidal 120
        Direction motor_direction = Clk_CW; // default motor direction is clockwise
        
        int pwm_freq = 20000; // default PWM frequency in Hz
        float voltage_limit = 12.0; // default voltage limit in Volts
        float voltage_power_supply = 12.0; // default power supply voltage in Volts
};