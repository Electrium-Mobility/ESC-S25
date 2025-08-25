#pragma once

/// Basic PID controller with output clamping, integral anti-windup and an
/// optional output slew limit.
class PID {
    public:
        PID(float kp, float ki, float kd, float output_min, float output_max, float output_ramp = 0.0f);

        /// Run one step. error = setpoint - measurement, dt in seconds.
        float update(float error, float dt);

        /// Clear the integrator and history (call when the loop is re-engaged).
        void reset(void);

        float kp;
        float ki;
        float kd;
        float output_min;
        float output_max;
        float output_ramp; // max change of the output per second, 0 disables

    private:
        float integral = 0.0f;
        float prev_error = 0.0f;
        float prev_output = 0.0f;
};
