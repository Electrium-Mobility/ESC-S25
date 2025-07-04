#define constraint(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define fast_round(x) (((x) >= 0) ? (long)((x) + 0.5) : (long)((x) - 0.5))