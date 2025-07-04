#include "BldcController.h"


void set_phase_voltage(float Uq, float Ud, float elec_angle){

    int sector;

    switch (control_mode) {
        case trapezoidal_120:
            trapezoidal_120_set_phase_voltage(Uq, Ud, elec_angle, &sector);
            break;
        case sinusoidal:
            // Implement sinusoidal control logic
            break;
        case space_vector:
            // Implement space vector control logic
            break;
        default:
            // Handle unsupported control mode
            break;
    }
}


void trapezoidal_120_set_phase_voltage(float Uq, float Ud, float elec_angle, int* sector) {
    // Implement trapezoidal 120 control logic

    // // each is 30 degrees with values for 3 phases of 1=positive -1=negative 0=high-impedance
    static int trapezoidal_120_map[12][3] = {
        {0,1,-1},{-1,1,-1},{-1,1,0},{-1,1,1},{-1,0,1},{-1,-1,1},{0,-1,1},{1,-1,1},{1,-1,0},{1,-1,-1},{1,0,-1},{1,1,-1} 
    };

    int local_sector = 6 * normalize_angle(elec_angle + M_PI / 6.0) / M_2_PI;
    if (sector) {
        *sector = local_sector;
    }
    
    Ua = Uq + trapezoidal_120_map[local_sector][0] * Uq;
    Ub = Uq + trapezoidal_120_map[local_sector][1] * Uq;
    Uc = Uq + trapezoidal_120_map[local_sector][2] * Uq;
        
    Ua += voltage_limit / 2 - Uq;
    Ub += voltage_limit / 2 - Uq;
    Uc += voltage_limit / 2 - Uq;
}


double normalize_angle(double angle) {
    while (angle > 360.0) angle -= 360.0;
    while (angle < 0.0) angle += 360.0;
    return angle;
}