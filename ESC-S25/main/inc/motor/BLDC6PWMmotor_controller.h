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

// #define HIGH_IMPEDANCE 0

FOC_MODULATION modulation = Trapezoid_120; // default modulation is trapezoidal 120

void UpdateBLDCMotor(float magnitude, double angle);  
void SetPhaseVoltage(double voltage_q, double voltage_d, double angle);

void NormalizeAngle(double angle);


