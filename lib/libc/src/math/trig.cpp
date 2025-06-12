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

