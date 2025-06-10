#ifdef __SSE__
#include <math.h>
#include <immintrin.h>
// #include <kdef.h>


double sqrt( double x )
{
    __m128d input  = _mm_set_sd(x);
    __m128d result = _mm_sqrt_sd(input, input);
    return _mm_cvtsd_f64(result);
}

float fsqrt( float x )
{
    return (float)sqrt((double)x);
}



#ifdef __AVX__
    double floor( double x )
    {
        // return double(int(x));
        __m128d input  = _mm_set1_pd(x);
        __m128d result = _mm_floor_pd(input);
        return _mm_cvtsd_f64(result);
    }

    double ceil( double x )
    {
        // return double(int(x+1.0));
        __m128d input  = _mm_set1_pd(x);
        __m128d result = _mm_ceil_pd(input);
        return _mm_cvtsd_f64(result);
    }

    double round( double x )
    {
        // return double(int(x+0.5));
        __m128d input  = _mm_set1_pd(x);
        __m128d result = _mm_round_pd(input, _MM_FROUND_TO_NEAREST_INT);
        return _mm_cvtsd_f64(result);
    }

    double trunc( double x )
    {
        // return (x < 0.0) ? ceil(x) : floor(x);
        __m128d input  = _mm_set1_pd(x);
        __m128d result = _mm_round_pd(input, _MM_FROUND_TRUNC);
        return _mm_cvtsd_f64(result);
    }

    double fract( double x )
    {
        return x - floor(x);
    }
#endif

// double log( double x )
// {
//     double rx = sqrt(x);
//     return rx - (1.0 / rx);
// }


// double exp( double x )
// {
//     double inv = 1.0;
//     double x_k = x;

//     inv = inv - (x_k) / 1.0;    x_k *= x;
//     inv = inv + (x_k) / 2.0;    x_k *= x;
//     inv = inv + (x_k) / 6.0;    x_k *= x;
//     inv = inv + (x_k) / 24.0;   x_k *= x;
//     inv = inv + (x_k) / 120.0;  x_k *= x;

//     return 1.0 / inv;

//     // double inv = 1.0;
//     // double dir = -1.0;

//     // double k_factorial = 1.0;

//     // // k1: 1
//     // // k2: k1*2 ==  1*2 == 2
//     // // k3: k2*3 ==  2*3 == 6
//     // // k4: k3*4 ==  6*4 == 24
//     // // k5: k4*5 == 24*5 == 120

//     // for (int k=1; k<=8; k++)
//     // {
//     //     k_factorial = k_factorial * (double)k;

//     //     inv += dir * (1.0 / k_factorial);
//     //     dir *= -1.0;
//     // }

//     // return inv * x;
// }


// double pow( double x, double y )
// {
//     union {
//         double d;
//         int a[2];
//     } u = { x };

//     u.a[1] = (int)(y * (u.a[1] - 1072632447) + 1072632447);
//     u.a[0] = 0;

//     return u.d;
//     // return exp(y*log(x));
// }


// double fmod( double x, double y )
// {
//     return x - y * trunc(x/y);
// }


// double fabs( double x )
// {
//     union {
//         double f;
//         uint64_t i;
//     } u = {x};

// 	u.i &= -1ULL / 2;

// 	return u.f;
// }


#endif
