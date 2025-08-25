#include "PID.h"

PID::PID(float kp, float ki, float kd, float output_min, float output_max, float output_ramp)
    : kp(kp), ki(ki), kd(kd), output_min(output_min), output_max(output_max), output_ramp(output_ramp) {}

float PID::update(float error, float dt) {
    if (dt <= 0.0f) {
        return prev_output;
    }

    float p = kp * error;

    // Trapezoidal integration, clamped so the I term alone can't exceed the output range
    integral += ki * dt * 0.5f * (error + prev_error);
    if (integral > output_max) integral = output_max;
    if (integral < output_min) integral = output_min;

    float d = kd * (error - prev_error) / dt;

    float out = p + integral + d;
    if (out > output_max) out = output_max;
    if (out < output_min) out = output_min;

    if (output_ramp > 0.0f) {
        float max_step = output_ramp * dt;
        if (out - prev_output > max_step) out = prev_output + max_step;
        else if (out - prev_output < -max_step) out = prev_output - max_step;
    }

    prev_error = error;
    prev_output = out;
    return out;
}

void PID::reset(void) {
    integral = 0.0f;
    prev_error = 0.0f;
    prev_output = 0.0f;
}
