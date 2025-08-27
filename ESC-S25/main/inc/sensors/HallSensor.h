#pragma once

#include <stdint.h>

/// Three-hall-sensor rotor position and speed estimate. The hall lines are
/// polled, so call update() often (every control tick is plenty).
class HallSensor {
    public:
        HallSensor(int pin_a, int pin_b, int pin_c, int pole_pairs = 1);

        void init(void);

        /// Sample the pins and update angle / velocity. Call periodically.
        void update(void);

        /// Electrical angle in radians [0, 2*PI), centred in the current 60 degree sector.
        float getElectricalAngle(void) const { return electrical_angle; }

        /// Electrical velocity in rad/s, signed. Zero if no edge has been seen recently.
        float getElectricalVelocity(void) const { return electrical_velocity; }

        /// Mechanical velocity in rad/s (electrical velocity / pole pairs).
        float getMechanicalVelocity(void) const { return electrical_velocity / pole_pairs; }

        /// False until the first valid reading; also false for the invalid 000 / 111 codes.
        bool isValid(void) const { return valid; }

        /// Flip if the motor phases are wired so the hall sequence runs backwards.
        bool reversed = false;

    private:
        int pin_a, pin_b, pin_c;
        int pole_pairs;
        int last_sector = -1;
        int64_t last_edge_us = 0;
        float electrical_angle = 0.0f;
        float electrical_velocity = 0.0f;
        bool valid = false;
};
