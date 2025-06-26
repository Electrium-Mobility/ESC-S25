//         FOC_MODULATION modulation; // default modulation is trapezoidal 120
#include "math.h"
#include "BLDC6PWMmcpwm.h"
#include "BLDC6PWMDriver.h"
#include "BLDCMotor.h" // Add this line to include the base class definition


#define PI 3.14159265358979323846


typedef enum
{
    Trapezoid_120 = 0,
    Sinusoidal = 1,
    Space_Vector = 2,
} FOC_MODULATION;

// #define HIGH_IMPEDANCE 0



class Bldc6PwmMotorController : public BLDCMotor {
    public:
        Bldc6PwmMotorController(int pole_pairs);

        BLDCDriver *driver; // Pointer to the driver

        void DriverInit(BLDC6PWMDriver *driver);

        void ControllerInit(void);

        void Update(void);

        void SetPhaseVoltage(double voltage_q, double voltage_d, double angle);

    
};
 // default modulation is trapezoidal 120



