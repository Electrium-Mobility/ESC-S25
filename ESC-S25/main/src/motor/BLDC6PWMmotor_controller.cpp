#include "BLDC6PWMmotor_controller.h"



void UpdateBLDCMotor(float magnitude, double angle){
    
}



void SetPhaseVoltage(double voltage_q, double voltage_d, double angle){

    int sector = 0;

    int Va, Vb, Vc;

    double voltage_limit = DEFAULT_VOLTAGE_POWER_SUPPLY; // This should be set to the maximum voltage limit of the motor driver
    
    switch(modulation) {
        case Trapezoid_120:
            // Trapezoidal modulation

            // 1 = postive, -1 = negative, 0 = high-impedance
            static int trap_120_map[6][3] = {
            {0,1,-1},{-1,1,0},{-1,0,1},{0,-1,1},{1,-1,0},{1,0,-1} // each is 60 degrees with values for 3 phases of 1=positive -1=negative 0=high-z
            };
            // static int trap_120_state = 0;
            sector = 6 * (fmod(angle + PI/6.0, 2 * PI) / (2 * PI)); // adding PI/6 to align with other modes

            Va = voltage_q + trap_120_map[sector][0] * voltage_q;
            Vb = voltage_q + trap_120_map[sector][1] * voltage_q;
            Vc = voltage_q + trap_120_map[sector][2] * voltage_q;


            Va += voltage_limit / 2 - voltage_q;
            Vb += voltage_limit / 2 - voltage_q;
            Vc += voltage_limit / 2 - voltage_q;

            break;
        case Sinusoidal:
            // Sinusoidal modulation

            
            break;
        case Space_Vector:
            // Space vector modulation
            break;
        default:
            ESP_LOGE("BLDC6PWMmotor_controller", "Unknown modulation type");
            return;
    }
}


double NormalizeAngle(double angle) {
    // Normalize the angle to the range [0, 2*PI)
    while (angle < 0) {
        angle += 2 * PI;
    }
    while (angle >= 2 * PI) {
        angle -= 2 * PI;
    }
    return angle;
}