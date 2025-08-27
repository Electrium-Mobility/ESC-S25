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

/// Apply a voltage vector of the given magnitude (q axis) at an electrical angle.
void UpdateBLDCMotor(float magnitude, double angle);

/// Write the phase voltages for the active modulation. voltage_q / voltage_d are
/// clamped so the result never exceeds what the supply can deliver.
void SetPhaseVoltage(double voltage_q, double voltage_d, double angle);

/// Open-loop speed control. Integrates the electrical angle at target_velocity
/// (electrical rad/s, negative reverses) using the real time since the last call.
void VelocityOpenLoop(float target_velocity, float voltage);

/// Reset the open-loop angle integrator (call before starting from standstill).
void ResetOpenLoop(void);

/// Cut the output: zero voltage on every phase.
void StopMotor(void);

/// Wrap an angle in radians into [0, 2*PI).
double NormalizeAngle(double angle);
