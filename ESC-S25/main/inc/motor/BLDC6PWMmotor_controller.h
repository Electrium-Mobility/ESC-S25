#pragma once

#include "BLDC6PWMmcpwm.h"
#include "BLDCDriver.h"

#define PI 3.14159265358979323846

typedef enum
{
    Trapezoid_120 = 0,
    Sinusoidal = 1,
    Space_Vector = 2,
} FOC_MODULATION;

// Active modulation, default is trapezoidal 120 (defined in BLDC6PWMmotor_controller.cpp)
extern FOC_MODULATION modulation;

/// Select the driver that SetPhaseVoltage() writes to.
void AttachDriver(BLDCDriver *driver);

void UpdateBLDCMotor(float magnitude, double angle);
void SetPhaseVoltage(double voltage_q, double voltage_d, double angle);

/// Wrap an angle in radians into [0, 2*PI).
double NormalizeAngle(double angle);
