#include "BldcController.h"
#include "Driver/Bldc6PwmDriver.h"

// Variable definitions for externs in BldcController.h
float shaft_angle = 0.0f; // Mechanical angle of the motor shaft in radians
int pole_pairs = 6; // Number of pole pairs in the motor
float voltage_q = 0.0f; // Voltage in the q-axis (quadrature axis)
float voltage_d = 0.0f; // Voltage in the d-axis (direct axis)
float Ua = 0.0f; // Phase A voltage
float Ub = 0.0f; // Phase B voltage
float Uc = 0.0f; // Phase C voltage
float duty_A = 0.0f; // Duty cycle for phase A
float duty_B = 0.0f; // Duty cycle for phase B
float duty_C = 0.0f; // Duty cycle for phase C

int sin_array[200] = {0,79,158,237,316,395,473,552,631,710,789,867,946,1024,1103,1181,1260,1338,1416,1494,1572,1650,1728,1806,1883,1961,2038,2115,2192,2269,2346,2423,2499,2575,2652,2728,2804,2879,2955,3030,3105,3180,3255,3329,3404,3478,3552,3625,3699,3772,3845,3918,3990,4063,4135,4206,4278,4349,4420,4491,4561,4631,4701,4770,4840,4909,4977,5046,5113,5181,5249,5316,5382,5449,5515,5580,5646,5711,5775,5839,5903,5967,6030,6093,6155,6217,6279,6340,6401,6461,6521,6581,6640,6699,6758,6815,6873,6930,6987,7043,7099,7154,7209,7264,7318,7371,7424,7477,7529,7581,7632,7683,7733,7783,7832,7881,7930,7977,8025,8072,8118,8164,8209,8254,8298,8342,8385,8428,8470,8512,8553,8594,8634,8673,8712,8751,8789,8826,8863,8899,8935,8970,9005,9039,9072,9105,9138,9169,9201,9231,9261,9291,9320,9348,9376,9403,9429,9455,9481,9506,9530,9554,9577,9599,9621,9642,9663,9683,9702,9721,9739,9757,9774,9790,9806,9821,9836,9850,9863,9876,9888,9899,9910,9920,9930,9939,9947,9955,9962,9969,9975,9980,9985,9989,9992,9995,9997,9999,10000,10000};



float normalize_radian_angle(float angle) {
    while (angle > _2_PI) angle -= _2_PI;
    while (angle < 0.0) angle += _2_PI;
    return angle;
}



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


float electrical_angle(float mechanical_angle, int pole_pairs) {
    // Convert mechanical angle to electrical angle
    return (mechanical_angle * pole_pairs);
}


void trapezoidal_120_set_phase_voltage(float Uq, float Ud, float elec_angle, int* sector) {
    // Implement trapezoidal 120 control logic

    // // each is 30 degrees with values for 3 phases of 1=positive -1=negative 0=high-impedance
    static int trapezoidal_120_map[12][3] = {
        {0,1,-1},{-1,1,-1},{-1,1,0},{-1,1,1},{-1,0,1},{-1,-1,1},{0,-1,1},{1,-1,1},{1,-1,0},{1,-1,-1},{1,0,-1},{1,1,-1} 
    };

    int local_sector = 6 * (normalize_radian_angle(elec_angle + _PI / 6.0) / _2_PI); // Convert electrical angle to sector (0-11)
    if (sector) {
        *sector = local_sector;
    }

    // ESP_LOGI("BLDC", "Sector: %d, Electrical Angle: %.2f rad", local_sector, elec_angle);
    // vTaskDelay(5 / portTICK_PERIOD_MS); // Delay for 10 milliseconds
    
    Ua = Uq + trapezoidal_120_map[local_sector][0] * Uq;
    Ub = Uq + trapezoidal_120_map[local_sector][1] * Uq;
        Uc = Uq + trapezoidal_120_map[local_sector][2] * Uq;
        
    Ua += voltage_limit / 2 - Uq;
    Ub += voltage_limit / 2 - Uq;
    Uc += voltage_limit / 2 - Uq;

    set_pwm(Ua, Ub, Uc, voltage_limit);
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


void loop(){
    set_phase_voltage(voltage_limit, 0, electrical_angle(shaft_angle, pole_pairs));
}

void move_to(float target){
    switch (foc_control_mode) {
        case FOC_CONTROL_VOLTAGE:
            voltage_q = target; // Set the target voltage in the q-axis
            break;
        case FOC_CONTROL_ANGLE:
            
            break;
        default:
            // Handle unsupported control mode
            break;
    }
}



void test_foc(void* arg){
    pwm_config(1000, 0.0001, 13, 14, 11, 12, 21, 47);

    while(1){
        // shaft_angle += _PI / 24; // Increment the shaft angle by 0.5 radians
        shaft_angle = fmod(shaft_angle, _2_PI); // Keep the angle within 0 to 2π


        // ESP_LOGI("BLDC", "Shaft Angle: %.2f rad", shaft_angle);
        // ESP_LOGI("BLDC", "Electrical Angle: %.2f rad", electrical_angle(shaft_angle, pole_pairs));

        loop();

        // vTaskDelay(10 / portTICK_PERIOD_MS); // Delay for 10 milliseconds
        // fflush(stdout);
    }
}