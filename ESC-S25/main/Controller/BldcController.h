#define M_PI 3.14159265359
#define M_2_PI 6.28318530718
#define M_3_PI 9.42477796077
#define M_3_PI_2 4.71238898038
#define M_PI_2 1.57079632679

#define SINE_SCALE_FACTOR    126.6873f   // scales radians to array index
#define SINE_INDEX_PI_2      199         // index at π/2 radians
#define SINE_INDEX_PI        398         // index at π radians
#define SINE_INDEX_2_PI       796         // index at 2π radians
#define SINE_SCALE_DOWN      0.0001f     // scale integer sine values to float


#define constraint(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

typedef enum{
    trapezoidal_120,
    sinusoidal,
    space_vector
} control_mode_t;

static const control_mode_t control_mode = trapezoidal_120;

static const float voltage_power_supply = 12.0; // Example power supply voltage
static const float voltage_limit = 12; // Example voltage limit for the motor driver

int sine_array[200] = {0,79,158,237,316,395,473,552,631,710,789,867,946,1024,1103,1181,1260,1338,1416,1494,1572,1650,1728,1806,1883,1961,2038,2115,2192,2269,2346,2423,2499,2575,2652,2728,2804,2879,2955,3030,3105,3180,3255,3329,3404,3478,3552,3625,3699,3772,3845,3918,3990,4063,4135,4206,4278,4349,4420,4491,4561,4631,4701,4770,4840,4909,4977,5046,5113,5181,5249,5316,5382,5449,5515,5580,5646,5711,5775,5839,5903,5967,6030,6093,6155,6217,6279,6340,6401,6461,6521,6581,6640,6699,6758,6815,6873,6930,6987,7043,7099,7154,7209,7264,7318,7371,7424,7477,7529,7581,7632,7683,7733,7783,7832,7881,7930,7977,8025,8072,8118,8164,8209,8254,8298,8342,8385,8428,8470,8512,8553,8594,8634,8673,8712,8751,8789,8826,8863,8899,8935,8970,9005,9039,9072,9105,9138,9169,9201,9231,9261,9291,9320,9348,9376,9403,9429,9455,9481,9506,9530,9554,9577,9599,9621,9642,9663,9683,9702,9721,9739,9757,9774,9790,9806,9821,9836,9850,9863,9876,9888,9899,9910,9920,9930,9939,9947,9955,9962,9969,9975,9980,9985,9989,9992,9995,9997,9999,10000,10000};


/// @brief Compute the sine of an angle in radians.
/// @param x The angle in radians.
/// @return The sine of the angle.
float fast_rad_sin(float x){
    if(x < M_PI_2){
    //return sine_array[(int)(199.0*( a / (_PI/2.0)))];
    //return sine_array[(int)(126.6873* a)];           // float array optimized
    return SINE_SCALE_DOWN*sine_array[round(SINE_SCALE_FACTOR * x)];      // int array optimized
  }else if(x < M_PI){
    // return sine_array[(int)(199.0*(1.0 - (a-M_PI_2) / (M_PI_2)))];
    //return sine_array[398 - (int)(126.6873*a)];          // float array optimized
    return SINE_SCALE_DOWN*sine_array[SINE_INDEX_PI - round(SINE_SCALE_FACTOR * x)];     // int array optimized
  }else if(x < M_3_PI_2){
    // return -sine_array[(int)(199.0*((a - _PI) / (_PI/2.0)))];
    //return -sine_array[-398 + (int)(126.6873*a)];           // float array optimized
    return -SINE_SCALE_DOWN*sine_array[-SINE_INDEX_PI + round(SINE_SCALE_FACTOR * x)];      // int array optimized
  } else {
    // return -sine_array[(int)(199.0*(1.0 - (a - 3*_PI/2) / (_PI/2.0)))];
    //return -sine_array[796 - (int)(126.6873*a)];           // float array optimized
    return -SINE_SCALE_DOWN*sine_array[SINE_INDEX_2_PI - round(SINE_SCALE_FACTOR * x)];      // int array optimized
  }
}

/// @brief Compute the cosine of an angle in radians.
/// @param x The angle in radians.
/// @return The cosine of the angle.
double fast_rad_cos(double x) {
    // cos(x) = sin(x + π/2)
    float x_sin = x + M_PI_2;
    x_sin = x_sin > M_2_PI ? x_sin - M_2_PI : x_sin; // Normalize to [0, 2π]
    return fast_rad_sin(x_sin);
}


/// @brief Normalize an angle to the range [0, 2π).
/// @param x The angle in radians.
/// @return The normalized angle in radians.
float normalize_angle(float x) {
     float a = fmod(x, M_2_PI);
  return a >= 0 ? a : (a + M_2_PI);
}


float electrical_angle(float mechanical_angle, int pole_pairs) {
    // Convert mechanical angle to electrical angle
    return (mechanical_angle * pole_pairs);
}





