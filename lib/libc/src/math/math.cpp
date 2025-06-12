#ifdef __SSE__
#include <math.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <bit>


double floor( double x )
{
    double ix = double(int64_t(x));
    if (x >= 0.0)
        return double(int64_t(x));
    return (x == ix) ? ix : ix-1.0;
}

double ceil( double x )
{
    double ix = double(int64_t(x));
        return (x == ix) ? ix : ix+1;
    return double(int64_t(x));
}

double round( double x )
{
    return (x >= 0.0) ? floor(x+0.5) : ceil(x-0.5);
}

double trunc( double x )
{
    return double(int64_t(x));
}

double fract( double x )
{
    return x - trunc(x);
}

double fabs( double x )
{
    double sign = (x < 0.0) ? -1.0 : +1.0;
    return sign*x;
}

double fmod( double x, double y )
{
    return x - (floor(x/y) * y);
}

// float floorf( float x )
// {
//     __m128 input  = _mm_set1_ps(x);
//     __m128 result = _mm_floor_ps(input);
//     return _mm_cvtss_f32(result);
// }



// double floor( double x )
// {
//     __m128d input  = _mm_set1_pd(x);
//     __m128d result = _mm_floor_pd(input);
//     return _mm_cvtsd_f64(result);
// }

// double ceil( double x )
// {
//     __m128d input  = _mm_set1_pd(x);
//     __m128d result = _mm_ceil_pd(input);
//     return _mm_cvtsd_f64(result);
// }

// double round( double x )
// {
//     __m128d input  = _mm_set1_pd(x);
//     __m128d result = _mm_round_pd(input, _MM_FROUND_TO_NEAREST_INT);
//     return _mm_cvtsd_f64(result);
// }

// double trunc( double x )
// {
//     __m128d input  = _mm_set1_pd(x);
//     __m128d result = _mm_round_pd(input, _MM_FROUND_TRUNC);
//     return _mm_cvtsd_f64(result);
// }

// double fract( double x )
// {
//     return x - trunc(x);
// }

// double sign( double x )
// {
//     if (x == 0.0)
//         return 0.0;
//     return (x < 0.0) ? -1.0 : +1.0;
// }

// double fabs( double x )
// {
//     return sign(x) * x;
// }

// double fmod( double x, double y )
// {
//     return x - (floor(x/y) * y);
// }



// double log_e( double x )
// {
//     // Extract exponent and mantissa
//     union {
//         double   f64;
//         uint64_t u64;
//     } U = {x};
//     uint64_t ix = U.u64;

//     int exponent = ((ix >> 52) & 0x7FF) - 1023; // Extract exponent (bias 1023)
//     ix = (ix & 0x000FFFFFFFFFFFFF) | 0x3FF0000000000000; // Normalize mantissa
//     double m = *(double*)&ix; // Mantissa (normalized)
    
//     // Polynomial or minimax approximation for ln(m) around m ~ 1.0
//     double m1 = m - 1.0;
//     double m2 = m1 * m1;
    
//     // Coefficients for minimax approximation
//     const double c1 = +0.999999999999996;
//     const double c2 = -0.499999999999999;
//     const double c3 = +0.333333333333333;
//     const double c4 = -0.250000000000000;
    
//     double log_m = m1 * (c1 + m1 * (c2 + m1 * (c3 + m1 * c4)));
//     double log_x = log_m + exponent * 0.6931471805599453; // ln(2) ~ 0.6931471805599453

//     return log_x;
// }



#endif
