#include "../util.h"
#include "Driver/Bldc6PwmDriver.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <math.h>
#include "esp_timer.h"
#include "esp_attr.h"
#include "esp_log.h"

#define _PI 3.14159265359
#define _2_PI 6.28318530718
#define _3_PI 9.42477796077
#define _3_PI_2 4.71238898038
#define _PI_2 1.57079632679

#define SIN_SCALE_FACTOR    126.6873f   // scales radians to array index
#define SIN_INDEX_PI_2      199         // index at π/2 radians
#define SIN_INDEX_PI        398         // index at π radians
#define SIN_INDEX_2_PI       796         // index at 2π radians
#define SIN_SCALE_DOWN      0.0001f     // scale integer sine values to float

typedef enum {
  TRAPEZOIDAL_120, // Trapezoidal control with 120-degree commutation
  FOC_SINUSOIDAL,  // Sinusoidal control
  SPACE_VECTOR     // Space vector control
} foc_modulation_mode_t;


typedef enum{
  FOC_CONTROL_VOLTAGE, // Voltage control mode
  FOC_CONTROL_ANGLE // Angle control mode
} foc_control_mode_t;

// FOC mode for the BLDC motor
static const foc_modulation_mode_t foc_modulation_mode = TRAPEZOIDAL_120;

// Control mode for the FOC algorithm
static const foc_control_mode_t foc_control_mode = FOC_CONTROL_VOLTAGE; // Can be voltage or angle control


static const float voltage_power_supply = 12.0; // Example power supply voltage

// default voltage limit is same as power supply voltage
// This can be changed to a lower value if needed
static const float voltage_limit = voltage_power_supply; // Example voltage limit for the motor driver

extern float shaft_angle; // Mechanical angle of the motor shaft in radians
extern int pole_pairs; // Number of pole pairs in the motor

extern float voltage_q; // Voltage in the q-axis (quadrature axis)
extern float voltage_d; // Voltage in the d-axis (direct axis)

extern float Ua; // Phase A voltage
extern float Ub; // Phase B voltage
extern float Uc; // Phase C voltage

extern float duty_A; // Duty cycle for phase A
extern float duty_B; // Duty cycle for phase B
extern float duty_C; // Duty cycle for phase C


//---------//
extern int sin_array[200]; // Sine lookup table for fast sine calculation

void test_foc(void* arg);

// Function declarations for BldcController.c
float normalize_radian_angle(float angle);
void trapezoidal_120_set_phase_voltage(float Uq, float Ud, float elec_angle, int* sector);
void set_phase_voltage(float Uq, float Ud, float elec_angle);
void loop(void);
void move_to(float target);
float fast_rad_sin(float x);
double fast_rad_cos(double x);
float normalize_angle(float x);
float electrical_angle(float mechanical_angle, int pole_pairs);

