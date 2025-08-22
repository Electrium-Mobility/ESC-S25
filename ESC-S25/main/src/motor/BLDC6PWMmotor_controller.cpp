#include "BLDC6PWMmotor_controller.h"
#include "DefaultValues.h"
#include <math.h>
#include "esp_log.h"
#include "esp_timer.h"

#define TAG "BLDC6PWMmotor_controller"

FOC_MODULATION modulation = Trapezoid_120;

static BLDCDriver *driver = nullptr;

// Open-loop state
static double open_loop_angle = 0.0;
static int64_t open_loop_timestamp_us = 0;

void AttachDriver(BLDCDriver *d) {
    driver = d;
}

void UpdateBLDCMotor(float magnitude, double angle){
    SetPhaseVoltage(magnitude, 0.0, angle);
}

void ResetOpenLoop(void) {
    open_loop_angle = 0.0;
    open_loop_timestamp_us = 0;
}

void VelocityOpenLoop(float target_velocity, float voltage){
    int64_t now = esp_timer_get_time();
    // First call after a reset has no previous timestamp, so don't move yet
    double Ts = (open_loop_timestamp_us == 0) ? 0.0 : (now - open_loop_timestamp_us) * 1e-6;
    open_loop_timestamp_us = now;

    open_loop_angle = NormalizeAngle(open_loop_angle + target_velocity * Ts);
    SetPhaseVoltage(voltage, 0.0, open_loop_angle);
}

void StopMotor(void){
    if (driver != nullptr) {
        // 50% duty on all three phases means no net voltage across the windings
        float mid = driver->voltage_power_supply / 2.0f;
        driver->setPwmVoltage(mid, mid, mid);
    }
}

void SetPhaseVoltage(double voltage_q, double voltage_d, double angle){
    if (driver == nullptr) {
        return;
    }

    float Va = 0, Vb = 0, Vc = 0;

    const double Vs = driver->voltage_power_supply;
    const double half = Vs / 2.0;

    // Largest vector each scheme can produce without clipping
    double max_vector = (modulation == Space_Vector) ? Vs / sqrt(3.0) : half;
    double mag = sqrt(voltage_q * voltage_q + voltage_d * voltage_d);
    if (driver->voltage_limit < max_vector) {
        max_vector = driver->voltage_limit;
    }
    if (mag > max_vector && mag > 0.0) {
        double scale = max_vector / mag;
        voltage_q *= scale;
        voltage_d *= scale;
    }

    switch(modulation) {
        case Trapezoid_120: {
            // Trapezoidal modulation

            // 1 = positive, -1 = negative, 0 = high-impedance
            static const int trap_120_map[6][3] = {
                {0,1,-1},{-1,1,0},{-1,0,1},{0,-1,1},{1,-1,0},{1,0,-1} // each is 60 degrees with values for 3 phases of 1=positive -1=negative 0=high-z
            };

            // adding PI/6 to align with other modes
            double a = NormalizeAngle(angle + PI/6.0);
            int sector = (int)(6 * (a / (2 * PI)));
            if (sector > 5) sector = 5;

            // Centre each phase on half the supply and offset by voltage_q
            Va = half + trap_120_map[sector][0] * voltage_q;
            Vb = half + trap_120_map[sector][1] * voltage_q;
            Vc = half + trap_120_map[sector][2] * voltage_q;
            break;
        }
        case Sinusoidal:
        case Space_Vector: {
            // Inverse Park transform (dq -> alpha/beta)
            double s = sin(angle);
            double c = cos(angle);
            double Ualpha = c * voltage_d - s * voltage_q;
            double Ubeta  = s * voltage_d + c * voltage_q;

            // Inverse Clarke transform (alpha/beta -> three phases)
            double Ua = Ualpha;
            double Ub = (-Ualpha + sqrt(3.0) * Ubeta) / 2.0;
            double Uc = (-Ualpha - sqrt(3.0) * Ubeta) / 2.0;

            double center = half;
            if (modulation == Space_Vector) {
                // Centre the three waveforms in the middle of the supply range (min/max
                // injection), which gives ~15% more usable voltage than plain sine
                double umax = fmax(Ua, fmax(Ub, Uc));
                double umin = fmin(Ua, fmin(Ub, Uc));
                center = half - (umax + umin) / 2.0;
            }
            Va = (float)(Ua + center);
            Vb = (float)(Ub + center);
            Vc = (float)(Uc + center);
            break;
        }
        default:
            ESP_LOGE(TAG, "Unknown modulation type");
            return;
    }

    driver->setPwmVoltage(Va, Vb, Vc);
}

double NormalizeAngle(double angle) {
    // Normalize the angle to the range [0, 2*PI)
    double a = fmod(angle, 2 * PI);
    if (a < 0) {
        a += 2 * PI;
    }
    return a;
}
