#include "BLDC6PWMmotor_controller.h"
#include "DefaultValues.h"
#include <math.h>
#include "esp_log.h"

#define TAG "BLDC6PWMmotor_controller"

FOC_MODULATION modulation = Trapezoid_120;

static BLDCDriver *driver = nullptr;

void AttachDriver(BLDCDriver *d) {
    driver = d;
}

// TODO: not implemented yet
void UpdateBLDCMotor(float magnitude, double angle){
    (void)magnitude;
    (void)angle;
}

void SetPhaseVoltage(double voltage_q, double voltage_d, double angle){
    (void)voltage_d; // only used by the (not yet implemented) sinusoidal / space vector modes

    float Va = 0, Vb = 0, Vc = 0;

    double voltage_limit = DEFAULT_VOLTAGE_POWER_SUPPLY; // This should be set to the maximum voltage limit of the motor driver

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
            Va = voltage_limit / 2 + trap_120_map[sector][0] * voltage_q;
            Vb = voltage_limit / 2 + trap_120_map[sector][1] * voltage_q;
            Vc = voltage_limit / 2 + trap_120_map[sector][2] * voltage_q;
            break;
        }
        case Sinusoidal:
            // Sinusoidal modulation (not implemented)
            ESP_LOGW(TAG, "Sinusoidal modulation not implemented");
            return;
        case Space_Vector:
            // Space vector modulation (not implemented)
            ESP_LOGW(TAG, "Space vector modulation not implemented");
            return;
        default:
            ESP_LOGE(TAG, "Unknown modulation type");
            return;
    }

    if (driver != nullptr) {
        driver->setPwmVoltage(Va, Vb, Vc);
    }
}

double NormalizeAngle(double angle) {
    // Normalize the angle to the range [0, 2*PI)
    double a = fmod(angle, 2 * PI);
    if (a < 0) {
        a += 2 * PI;
    }
    return a;
}
