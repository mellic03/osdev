#ifdef __SSE__
#include <math.h>
#include <emmintrin.h>
#include "fpu.hpp"
#include "exp_table.hpp"
#include <kmemxx.hpp>
#include <bit>

// static double libcmath_ exp7( double x )
// {
//     return (362880+x*(362880+x*(181440+x*(60480+x*(15120+x*(3024+x*(504+x*(72+x*(9+x)))))))))*2.75573192e-6;
// }

static float exp_fast( float )  __attribute__((used));
static float exp_accurate( float )  __attribute__((used));


double exp( double x )
{
    return exp_accurate(x);
    // return exp_accurate(trunc(x)) * exp_accurate(fract(x));
    // return exp_fast(trunc(x)) * exp_fast(fract(x));
}


double pow( double x, double y )
{
    return exp(y*logn(x));
}


double sqrt( double x )
{
    __m128d reg = _mm_sqrt_pd(_mm_set_sd(x));
    return _mm_cvtsd_f64(reg);
}


float sqrtf( float x )
{
    __m128 reg = _mm_sqrt_ps(_mm_set_ss(x));
    return _mm_cvtss_f32(reg);
}









static float exp_fast( float x )
{
    __m128  regx = _mm_set1_ps(x);
    __m128  a    = _mm_set1_ps(12102203.2f); // (1 << 23) / ln(2)
    __m128i b    = _mm_set1_epi32(127 * (1 << 23) - 486411);
    __m128  m87  = _mm_set1_ps(-87);

    // fast exponential function, regx should be in [-87, 87]
    __m128  mask = _mm_cmpge_ps(regx, m87);
    __m128i tmp1 = _mm_add_epi32(_mm_cvtps_epi32(_mm_mul_ps(a, regx)), b);
    __m128  tmp2 = _mm_and_ps(_mm_castsi128_ps(tmp1), mask);

    return _mm_cvtss_f32(tmp2);
}



/// @return True if \p x is a NAN.
static bool is_nan(float x)
{
    unsigned xb = std::bit_cast<unsigned, float>(x);
    xb >>= 23;
    return (xb & 0xff) == 0xff;
}

// Approximate the function \p exp in the range -0.004, 0.004.
// Q = fpminimax(exp(x), 5, [|D...|], [-0.0039, 0.0039])
static double approximate_exp_pol_around_zero(float x) {
    return 1 +
           x * (1 + x * (0.49999999999985944576508245518198236823081970214844 +
                         x * (0.166666666666697105281258473041816614568233489990234 +
                              x * (4.1666696240209417922972789938285131938755512237549e-2 +
                                   x * 8.3333337622652735310335714302709675393998622894287e-3))));
}


static float exp_accurate( float x )
{
    if (x >= 710) {
        return std::bit_cast<float, unsigned>(0x7f800000); // Inf
    } else if (x <= -710) {
        return 0;
    } else if (is_nan(x)) {
        return x;
    }

    // Split X into 3 numbers such that: x = I1 + (I2 << 8) + xt;
    int Int1 = int(x);
    x = x - Int1;
    int Int2 = int(x * 256);
    x = x - (float(Int2) / 256);

    return approximate_exp_pol_around_zero(x)
           * libc_math::EXP_TABLE[Int1 + 710]
           * libc_math::EXP_TABLE_r256[Int2 + 256];
}







#endif