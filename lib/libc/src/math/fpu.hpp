#pragma once

extern "C"
{
    extern float FPU_S0;
    extern float FPU_S1;
    extern float FPU_R0;
    extern float FPU_R1;
    extern void FPU_fsin();
    extern void FPU_log2();
    extern void FPU_tan();
    // extern double FPU_log2( const double toLog, const double toMul);
}



inline double FPU_cmd( void (*func)(), double S0=0.0, double S1=0.0 )
{
    FPU_S0 = (float)S0;
    FPU_S1 = (float)S1;
    func();
    return (double)FPU_R0;
}


