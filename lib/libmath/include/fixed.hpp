#pragma once
#include <stddef.h>
#include <stdint.h>


#define BITS_PER_BYTE 8
#define FRACTION_BITS 16 // 1 << 16 = 2^16 = 65536
#define FRACTION_DIVISOR (1 << FRACTION_BITS)
#define FRACTION_MASK (FRACTION_DIVISOR - 1) // 65535 (all LSB set, all MSB clear)


struct Fixed
{
    static constexpr size_t Shift = 16;
    static constexpr size_t Scale = (1<<Shift);

    union {
        uint32_t value;
        struct {
            uint16_t lo, hi;
        };
    };
    

    Fixed(): value(0) {  };
    Fixed( const Fixed &rhs ): value(rhs.value) {  };
    Fixed( Fixed &&rhs ): value(rhs.value) {  };
    Fixed( float f ): value(Scale*f) {  };

    Fixed operator=( Fixed rhs )
    {
        value = rhs.value;
        return *this;
    }

    Fixed &operator=( float f )
    {
        value = uint32_t(Scale*f);
        return *this;
    }

    Fixed operator+( const Fixed &rhs )
    {
        Fixed F(0);
        F.value = value + rhs.value;
        return F;
    }

    Fixed operator-( const Fixed &rhs )
    {
        Fixed F(0);
        F.value = value - rhs.value;
        return F;
    }

    Fixed operator*( const Fixed &rhs )
    {
        Fixed F(0);
        F.value = (value * rhs.value) / Scale;
        return F;
    }

    Fixed operator/( const Fixed &rhs )
    {
        // constexpr size_t SC = 10'000;
        // constexpr size_t A  = SC * 25;
        // constexpr size_t B  = SC * 2;
        // constexpr size_t AB = (SC*A) / B;

        Fixed F(0);
        F.value = (Scale*value) / rhs.value;
        return F;
    }

    // Fixed operator+( const Fixed &rhs )
    // {
    //     return Fixed(value + rhs.value);
    // }

    // Fixed operator-( const Fixed &rhs )
    // {
    //     return Fixed(value - rhs.value);
    // }

    // Fixed operator*( const Fixed &rhs )
    // {
    //     return Fixed((value * rhs.value) / (Scale*Scale));
    // }

    // Fixed operator/( const Fixed &rhs )
    // {
    //     return Fixed(((Scale*Scale*value) / (Scale*rhs.value)) / Scale);
    // }

    Fixed &operator+=( const Fixed &rhs )
    {
        *this = *this + rhs;
        return *this;
    }

    Fixed &operator-=( const Fixed &rhs )
    {
        *this = *this - rhs;
        return *this;
    }

    Fixed &operator*=( const Fixed &rhs )
    {
        *this = *this * rhs;
        return *this;
    }

    Fixed &operator/=( const Fixed &rhs )
    {
        *this = *this / rhs;
        return *this;
    }

};


// inline
// Fixed operator+( const Fixed &F, float u )
// {
//     Fixed Fu(0);  Fu.value = F.value+Fixed::Scale*u;  return F;
// }

// inline
// Fixed operator+( float u, const Fixed &F )
// {
//     Fixed Fu(0);  Fu.value = F.value+Fixed::Scale*u;  return F;
// }

// inline
// Fixed operator-( Fixed F, float u )
// {
//     Fixed Fu(0);  Fu.value = F.value-Fixed::Scale*u;  return F;
// }

// inline
// Fixed operator-( float u, Fixed F )
// {
//     Fixed Fu(0);  Fu.value = F.value-Fixed::Scale*u;  return F;
// }


