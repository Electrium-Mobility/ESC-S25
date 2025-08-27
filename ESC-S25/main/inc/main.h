#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/// GPIO ///

// MCPWM GPIOs
#define GPIO_MCPWM0A_OUT 18
#define GPIO_MCPWM0B_OUT 19
#define GPIO_MCPWM1A_OUT 21
#define GPIO_MCPWM1B_OUT 22
#define GPIO_MCPWM2A_OUT 23
#define GPIO_MCPWM2B_OUT 25
#define GPIO_MCPWM_CAP0_IN 26
#define GPIO_MCPWM_CAP1_IN 27
#define GPIO_MCPWM_CAP2_IN 32

// Optional gate driver enable pin
#define GPIO_DRIVER_ENABLE 4

// Throttle potentiometer, must be an ADC1 pin (GPIO34 = ADC1 channel 6, input only)
#define GPIO_THROTTLE_IN 34

/// Control ///

#define CONTROL_OPEN_LOOP 0   // spin at the commanded speed without feedback
#define CONTROL_CLOSED_LOOP 1 // hall sensors + PID on speed
#define CONTROL_MODE CONTROL_OPEN_LOOP

#define MOTOR_POLE_PAIRS 6
#define CONTROL_PERIOD_MS 10

#define MAX_ELECTRICAL_VELOCITY 300.0f // rad/s electrical at full throttle
#define MAX_ACCELERATION 150.0f        // rad/s^2 electrical, limits how fast the target moves
#define OPEN_LOOP_VOLTAGE 2.0f         // volts of drive in open loop
#define MAX_DRIVE_VOLTAGE 4.0f         // clamp on the closed loop output

#define THROTTLE_START 0.05f // throttle needed to start the motor
#define THROTTLE_STOP 0.02f  // below this the motor is switched off (hysteresis)

// Speed PID (output is volts, input is electrical rad/s). Tune on the real motor.
#define SPEED_PID_KP 0.02f
#define SPEED_PID_KI 0.1f
#define SPEED_PID_KD 0.0f
