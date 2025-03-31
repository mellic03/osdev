#pragma once
#include <AAA.h>
#include <immintrin.h>

typedef int v4si  __attribute__ ((vector_size (16)));
typedef int v8si  __attribute__ ((vector_size (32)));
typedef int v16si __attribute__ ((vector_size (64)));

typedef float    v4sf __attribute__ ((vector_size (16)));
typedef float    v8sf __attribute__ ((vector_size (32)));
typedef double   v4df __attribute__ ((vector_size (32)));
typedef uint64_t v4di __attribute__ ((vector_size (32)));

typedef uint8_t  u8x16 __attribute__ ((vector_size (16)));
typedef uint32_t u32x4 __attribute__ ((vector_size (16)));

void ree( void *d, const void *s, size_t n )
{
    v8sf *dst = (v8sf*)d;
    v8sf *src = (v8sf*)s;

    for (size_t i=0; i<n/sizeof(v8sf); i++)
    {
        *(dst++) = *(src++);
    }

}

// void vxsi_example()
// {
//     v4si X, Y;
//     v4si Z = X + Y;

//     v4si A     = {1,2,3,4};
//     v4si B     = {5,6,7,8};
//     v4si mask1 = {0,1,1,3};
//     v4si mask2 = {0,4,2,5};
//     v4si res;
    
//     res = __builtin_shuffle (A, mask1);       /* res is {1,2,2,4}  */
//     res = __builtin_shuffle (A, B, mask2);    /* res is {1,5,3,6}  */


//     v8si a = {1,-2,3,-4,5,-6,7,-8};
//     v4si b = __builtin_shufflevector (a, a, 0, 2, 4, 6); /* b is {1,3,5,7} */
//     v4si c = {-2,-4,-6,-8};
//     v8si d = __builtin_shufflevector (c, b, 4, 0, 5, 1, 6, 2, 7, 3); /* d is a */

//     union {
//         __m128i mm;
//         u8x16   u8;
//         u32x4   u32;
//     } v128;
// }




#include <ZZZ.h>
