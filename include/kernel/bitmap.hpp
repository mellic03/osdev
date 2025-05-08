
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <array>


namespace idk
{
    template <size_t N>
    class BitMap;
}



/**
 * N - number of bits
 */
template <size_t N>
class idk::BitMap
{
private:

public:
    std::array<uint8_t, N/8> m_data;
    size_t m_setcount = 0;


    /**
     * @param data base pointer
     * @param size size of data in bytes
     */
    BitMap()
    {
        static_assert((N%8 == 0), "N must be a multiple of 8");
        clear();
    }

    void clear()
    {
        for (size_t i=0; i<N/8; i++)
        {
            m_data[i] = 0;
        }
    }

    size_t nbytes()
    {
        return N/8;
    }

    size_t nbits()
    {
        return N;
    }

    uint8_t get( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
    
        // if (idx > nbytes())
        // {
        //     std::cerr << "Index out of bounds\n";
        //     exit(1);
        // }

        return (m_data[idx] & (1ULL << bit)) != 0;
    }

    void set( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
    
        // if (idx > nbytes())
        // {
        //     std::cerr << "Index out of bounds\n";
        //     exit(1);
        // }

        m_data[idx] |= (1ULL << bit);
        m_setcount++;
    }

    void unset( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
    
        // if (idx > nbytes())
        // {
        //     std::cerr << "Index out of bounds\n";
        //     exit(1);
        // }

        m_data[idx] &= ~(1ULL << bit);
        m_setcount--;
    }

    constexpr bool is_set   ( size_t i ) { return  get(i); }
    constexpr bool is_unset ( size_t i ) { return !get(i); }

};


