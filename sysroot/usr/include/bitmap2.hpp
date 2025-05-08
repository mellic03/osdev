
#pragma once

#include <stddef.h>
#include <stdint.h>
// #include <array>


// namespace idk
// {
//     class BitMap2;
// }




// class idk::BitMap2
// {
// public:
//     size_t m_nbytes;
//     size_t m_nbits;
//     size_t m_setcount;
//     uint8_t *m_data;

//     BitMap2()
//     :   BitMap2(nullptr, 0)
//     {

//     };

//     BitMap2( uint8_t *base, size_t capacity )
//     :   m_nbytes(capacity/8),
//         m_nbits(capacity),
//         m_setcount(0),
//         m_data(base)
//     {
//         clear();
//     }

//     void clear()
//     {
//         for (size_t i=0; i<m_nbytes; i++)
//         {
//             m_data[i] = 0;
//         }
//     }

//     size_t nbytes()
//     {
//         return m_nbytes;
//     }

//     size_t nbits()
//     {
//         return m_nbits;
//     }

//     uint8_t get( size_t i )
//     {
//         size_t idx = i / 8;
//         size_t bit = i % 8;
//         return (m_data[idx] & (1ULL << bit)) != 0;
//     }

//     void set( size_t i )
//     {
//         size_t idx = i / 8;
//         size_t bit = i % 8;
//         m_data[idx] |= (1ULL << bit);
//         m_setcount++;
//     }

//     void unset( size_t i )
//     {
//         size_t idx = i / 8;
//         size_t bit = i % 8;
//         m_data[idx] &= ~(1ULL << bit);
//         m_setcount--;
//     }

//     constexpr bool is_set   ( size_t i ) { return  get(i); }
//     constexpr bool is_unset ( size_t i ) { return !get(i); }

// };

struct idk_BitMap
{
    size_t  nbits;
    size_t  setcount;
    uint8_t data[64];

    void clear()
    {
        for (size_t i=0; i<nbits; i++)
            unset(i);
        setcount = 0;
    }

    size_t nbytes()
    {
        return (nbits + 7) / 8;
    }

    uint8_t get( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        return (data[idx] & (1ULL << bit)) != 0;
    }

    void set( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        data[idx] |= (1ULL << bit);
        setcount++;
    }

    void unset( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        data[idx] &= ~(1ULL << bit);
        setcount--;
    }

    constexpr bool is_set   ( size_t i ) { return  get(i); }
    constexpr bool is_unset ( size_t i ) { return !get(i); }
};


