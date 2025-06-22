#ifdef __SSE__

#include <math.h>
#include <string.h>
#include "fpu.hpp"

double sin( double x )
{
    FPU_S0 = x;
    FPU_fsin();
    return FPU_R0;
}


#include "trig_table.hpp"

double sin_lookup( double x )
{
    double deg = (x / M_PI) * 180.0;
    int    idx = int(floor(deg)) % 360;
    double lo  = libc_math::sin_table[(idx + 0) % 360];
    double hi  = libc_math::sin_table[(idx + 1) % 360];
    double a   = deg - floor(deg);
    return (1.0 - a)*lo + a*hi;
}


double cos( double x )
{
    return sin(x + M_PI_4);
}

// float sinf( float x )
// {
//     return FPU_cmd(FPU_fsin, x);
// }


double tan( double x )
{
    return sin(x);
}




#endif

