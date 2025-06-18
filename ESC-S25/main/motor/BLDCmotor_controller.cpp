#include "BLDCmotor_controller.h"

int trap_120_map[6][3] = {
    {HIGH_IMPEDANCE,1,-1},
    {-1,1,HIGH_IMPEDANCE},
    {-1,HIGH_IMPEDANCE,1},
    {HIGH_IMPEDANCE,-1,1},
    {1,-1,HIGH_IMPEDANCE},
    {1,HIGH_IMPEDANCE,-1} 
};

typedef enum
{
    Trapezoid_120 = 0,
    Sinusoidal = 1
} FOC_MODULATION;







