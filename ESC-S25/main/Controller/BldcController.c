#include "BldcController.h"
#include "Driver/Bldc6PwmDriver.h"


#define A_H 35
#define A_L 36
#define B_H 37
#define B_L 38
#define C_H 39
#define C_L 40

#define POT_GPIO 36 // GPIO for potentiometer input

// Variable definitions for externs in BldcController.h
float shaft_angle = 0.0f; // Mechanical angle of the motor shaft in radians
int pole_pairs = 6; // Number of pole pairs in the motor
float voltage_q = 0.0f; // Voltage in the q-axis (quadrature axis)
float voltage_d = 0.0f; // Voltage in the d-axis (direct axis)
float Ua_H = 0.0f; // Phase A high-side voltage
float Ua_L = 0.0f; // Phase A low-side voltage
float Ub_H = 0.0f; // Phase B high-side voltage
float Ub_L = 0.0f; // Phase B low-side voltage
float Uc_H = 0.0f; // Phase C high-side voltage
float Uc_L = 0.0f; // Phase C low-side voltage
float duty_A_H = 0.0f; // Duty cycle for phase A high-side
float duty_A_L = 0.0f; // Duty cycle for phase A low-side
float duty_B_H = 0.0f; // Duty cycle for phase B high-side
float duty_B_L = 0.0f; // Duty cycle for phase B low-side
float duty_C_H = 0.0f; // Duty cycle for phase C high-side
float duty_C_L = 0.0f; // Duty cycle for phase C low-side

float pot_angle = 0;

long open_loop_timestamp = 0; // Timestamp for open loop control

// int sin_array[200] = {0,79,158,237,316,395,473,552,631,710,789,867,946,1024,1103,1181,1260,1338,1416,1494,1572,1650,1728,1806,1883,1961,2038,2115,2192,2269,2346,2423,2499,2575,2652,2728,2804,2879,2955,3030,3105,3180,3255,3329,3404,3478,3552,3625,3699,3772,3845,3918,3990,4063,4135,4206,4278,4349,4420,4491,4561,4631,4701,4770,4840,4909,4977,5046,5113,5181,5249,5316,5382,5449,5515,5580,5646,5711,5775,5839,5903,5967,6030,6093,6155,6217,6279,6340,6401,6461,6521,6581,6640,6699,6758,6815,6873,6930,6987,7043,7099,7154,7209,7264,7318,7371,7424,7477,7529,7581,7632,7683,7733,7783,7832,7881,7930,7977,8025,8072,8118,8164,8209,8254,8298,8342,8385,8428,8470,8512,8553,8594,8634,8673,8712,8751,8789,8826,8863,8899,8935,8970,9005,9039,9072,9105,9138,9169,9201,9231,9261,9291,9320,9348,9376,9403,9429,9455,9481,9506,9530,9554,9577,9599,9621,9642,9663,9683,9702,9721,9739,9757,9774,9790,9806,9821,9836,9850,9863,9876,9888,9899,9910,9920,9930,9939,9947,9955,9962,9969,9975,9980,9985,9989,9992,9995,9997,9999,10000,10000};

float normalize_radian_angle(float angle) {
    while (angle > _2_PI) angle -= _2_PI;
    while (angle < 0.0) angle += _2_PI;
    return angle;
}

/*
/// @brief Compute the sine of an angle in radians.
/// @param x The angle in radians.
/// @return The sine of the angle.
float fast_rad_sin(float x){
    if(x < _PI_2){
    //return sin_array[(int)(199.0*( a / (_PI/2.0)))];
    //return sin_array[(int)(126.6873* a)];           // float array optimized
    return SIN_SCALE_DOWN*sin_array[fast_round(SIN_SCALE_FACTOR * x)];      // int array optimized
  }else if(x < _PI){
    // return sin_array[(int)(199.0*(1.0 - (a-M_PI_2) / (M_PI_2)))];
    //return sin_array[398 - (int)(126.6873*a)];          // float array optimized
    return SIN_SCALE_DOWN*sin_array[SIN_INDEX_PI - fast_round(SIN_SCALE_FACTOR * x)];     // int array optimized
  }else if(x < _3_PI_2){
    // return -sin_array[(int)(199.0*((a - _PI) / (_PI/2.0)))];
    //return -sin_array[-398 + (int)(126.6873*a)];           // float array optimized
    return -SIN_SCALE_DOWN*sin_array[-SIN_INDEX_PI + fast_round(SIN_SCALE_FACTOR * x)];      // int array optimized
  } else {
    // return -sin_array[(int)(199.0*(1.0 - (a - 3*_PI/2) / (_PI/2.0)))];
    //return -sin_array[796 - (int)(126.6873*a)];           // float array optimized
    return -SIN_SCALE_DOWN*sin_array[SIN_INDEX_2_PI - fast_round(SIN_SCALE_FACTOR * x)];      // int array optimized
  }
}

/// @brief Compute the cosine of an angle in radians.
/// @param x The angle in radians.
/// @return The cosine of the angle.
double fast_rad_cos(double x) {
    // cos(x) = sin(x + π/2)
    float x_sin = x + _PI_2;
    x_sin = x_sin > _2_PI ? x_sin - _2_PI : x_sin; // Normalize to [0, 2π]
    return fast_rad_sin(x_sin);
}
*/

float electrical_angle(float mechanical_angle, int pole_pairs) {
    // Convert mechanical angle to electrical angle
    return (mechanical_angle * pole_pairs);
}

void trapezoidal_120_set_phase_voltage(float Uq, float Ud, float angle, int* sector) {
    // Implement trapezoidal 120 control logic

    // // each is 30 degrees with values for 3 phases of 1=positive 0=negative -1=high-impedance (downwards), -2=high-impedance (upwards)
    static int trap_120_map[6][3] = {
        {1, 0, -1},{1,-2,0},{-1,1,0},{0,1,-2},{0,-1,1},{-2,0,1} // each is 60 degrees with values for 3 phases of 1=positive -1=high-z 0=negative
    };
    // int local_sector = 6 * (normalize_radian_angle(elec_angle + _PI / 6.0)); // Convert electrical angle to sector (0-11)
    
    // float percent_angle = elec_angle % _2_PI; // Ensure electrical angle is within [0, 2π]

    int local_sector = angle / _PI_3; // Convert electrical angle to sector (0-5)
    float degree_angle = angle * (180.0 / _PI); // Convert radians to degrees


    if (sector) {
        *sector = local_sector;
    }

    float percent_angle = (float)fmod(angle, _PI_3);  // Ensure electrical angle is within [0, 2π]


    // ESP_LOGI("BLDC", "Sector: %d, Electrical Angle: %.2f rad", local_sector, elec_angle);
    // vTaskDelay(5 / portTICK_PERIOD_MS); // Delay for 10 milliseconds

    /*
    L H SH
    0 0 HI-z
    0 1 H
    1 0 L
    1 1 Hi-z
    
    */


    if (trap_120_map[local_sector][0] == -1){
        if (percent_angle < 0.5) {
            Ua_H = 1 - 2 * percent_angle;
            Ua_L = 0;
        } else {
            Ua_H = 0;
            Ua_L = percent_angle * 2 - 1;
        }
    }
    else if(trap_120_map[local_sector][0] == -2){
        if (percent_angle < 0.5) {
            Ua_H = 0;
            Ua_L = 1 - 2 * percent_angle;
        } else {
            Ua_H = 2 * percent_angle - 1;
            Ua_L = 0;
        }
    }
    else {
        Ua_H = trap_120_map[local_sector][0];
    }


    if (trap_120_map[local_sector][1] == -1){
        if (percent_angle < 0.5) {
            Ub_H = 1 - 2 * percent_angle;
            Ub_L = 0;
        } else {
            Ub_H = 0;
            Ub_L = percent_angle * 2 - 1;
        }
    }
    else if(trap_120_map[local_sector][1] == -2){
        if (percent_angle < 0.5) {
            Ub_H = 0;
            Ub_L = 1 - 2 * percent_angle;
        } else {
            Ub_H = 2 * percent_angle - 1;
            Ub_L = 0;
        }
    }
    else {
        Ub_H = trap_120_map[local_sector][1];
    }

    if (trap_120_map[local_sector][2] == -1){
        if (percent_angle < 0.5) {
            Uc_H = 1 - 2 * percent_angle;
            Uc_L = 0;
        } else {
            Uc_H = 0;
            Uc_L = percent_angle * 2 - 1;
        }
    }
    else if(trap_120_map[local_sector][2] == -2){
        if (percent_angle < 0.5) {
            Uc_H = 0;
            Uc_L = 1 - 2 * percent_angle;
        } else {
            Uc_H = 2 * percent_angle - 1;
            Uc_L = 0;
        }
    }
    else {
        Uc_H = trap_120_map[local_sector][2];
    }


    Ua_H = constraint(Ua_H, 0.0f, 1.0f);
    Ua_L = constraint(Ua_L, 0.0f, 1.0f);
    Ub_H = constraint(Ub_H, 0.0f, 1.0f);
    Ub_L = constraint(Ub_L, 0.0f, 1.0f);
    Uc_H = constraint(Uc_H, 0.0f, 1.0f);
    Uc_L = constraint(Uc_L, 0.0f, 1.0f);

    printf("Sector: %d \t, Rad Angle: %.2f \t, Degree Angle: %.2f \t, Percent Angle: %.2f \t, UaH: %.2f \t, UaL: %.2f \n", local_sector, angle, degree_angle, percent_angle, Ua_H, Ua_L);

    
    if (Ua_L < 0 || Ua_L > 1){
        ESP_LOGI("BLDC", "Ua_L out of bounds: %.2f", Ua_L);
    }


    // printf("Before set_pwm\n");
    pwm_write(Ua_H, Ua_L, Ub_H, Ub_L, Uc_H, Uc_L);
    // printf("After set_pwm\n");
}

void set_phase_voltage(float Uq, float Ud, float elec_angle){

    int sector;

    switch (foc_modulation_mode) {
        case TRAPEZOIDAL_120:
            trapezoidal_120_set_phase_voltage(Uq, Ud, elec_angle, &sector);
            break;
        case FOC_SINUSOIDAL:
            // Implement sinusoidal control logic
            break;
        case SPACE_VECTOR:
            // Implement space vector control logic
            break;
        default:
            // Handle unsupported control mode
            break;
    }
}


// void loop(){
//     set_phase_voltage(voltage_limit, 0, electrical_angle(shaft_angle, pole_pairs));
// }


void vel_open_loop(float target_velocity) {
    // get current timestamp
    long now_us = micros();
    // calculate the sample time from last call
    float Ts = (now_us - open_loop_timestamp) * 1e-6;
    // printf("Ts: %.6f seconds, now: %ld, past: %ld\n", Ts, now_us, open_loop_timestamp);
    // calculate the necessary angle to achieve target velocity
    shaft_angle += target_velocity*Ts; 

    float temp_angle = normalize_radian_angle(shaft_angle); // Ensure shaft angle is within [0, 2π]

    // printf("Shaft Angle: %.2f rad, Target Velocity: %.2f\n", temp_angle, target_velocity);

    // set the maximal allowed voltage (voltage_limit) with the necessary angle
    // set_phase_voltage(voltage_limit,  0, electrical_angle(temp_angle, pole_pairs));
    set_phase_voltage(voltage_limit,  0, temp_angle);

    // save timestamp for next call
    open_loop_timestamp = now_us;
}

long micros() {
    return (long)esp_timer_get_time();
}
// void move_to(float target){
//     switch (foc_control_mode) {
//         case FOC_CONTROL_VOLTAGE:
//             voltage_q = target; // Set the target voltage in the q-axis
//             break;
//         case FOC_CONTROL_ANGLE:
            
//             break;
//         default:
//             // Handle unsupported control mode
//             break;
//     }
// }


void foc_init(){
    pwm_config(40000, A_H, A_L, B_H, B_L, C_H, C_L);

}
