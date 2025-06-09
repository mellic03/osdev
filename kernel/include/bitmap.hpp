
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <array>


namespace idk
{
    template <size_t N>
    class BitMap;

    template <typename T, size_t Capacity, size_t Alignment=16>
    struct BitMapAllocator;

    template <typename T, size_t Capacity>
    struct BitMapAllocator2;
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
        for (size_t i=0; i<nbits(); i++)
            unset(i);
        m_setcount = 0;
    }

    size_t nbytes()
    {
        return (nbits() + 7) / 8;
    }

    size_t nbits()
    {
        return N;
    }

    uint8_t get( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        return (m_data[idx] & (1ULL << bit)) != 0;
    }

    void set( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        m_data[idx] |= (1ULL << bit);
        m_setcount++;
    }

    void unset( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        m_data[idx] &= ~(1ULL << bit);
        m_setcount--;
    }

    constexpr bool is_set   ( size_t i ) { return  get(i); }
    constexpr bool is_unset ( size_t i ) { return !get(i); }

};




template <typename T, size_t Capacity, size_t Alignment>
struct idk::BitMapAllocator
{
    idk::BitMap<Capacity> bmap;
    T data[Capacity] __attribute__((aligned(Alignment)));

    int64_t alloc()
    {
        for (size_t i=0; i<bmap.nbits(); i++)
        {
            if (bmap.is_unset(i))
            {
                bmap.set(i);
                new (get(i)) T();
                return i;
            }
        }
        return -1;
    }

    void free( int id ) { bmap.unset(id); }
    T   *get ( int id ) { return &data[id]; }
    void clear() { bmap.clear(); }
};






template <typename T, size_t Capacity>
struct idk::BitMapAllocator2
{
    idk::BitMap<Capacity> bmap;
    // T m_data[Capacity];
    uint8_t m_data[Capacity * sizeof(T)] __attribute__((aligned(16)));

    BitMapAllocator2()
    {
        bmap.clear();
    }

    template <typename... Args>
    T *alloc( Args... args )
    {
        for (size_t i=0; i<bmap.nbits(); i++)
        {
            if (bmap.is_unset(i))
            {
                bmap.set(i);
                T *ptr = (T*)(m_data + i*sizeof(T));
                return new (ptr) T(args...);
                // return new (&m_data[i]) T(args...);
            }
        }
        return nullptr;
    }

    void free( T *ptr )
    {
        ptr->~T();
        size_t idx = ((uint8_t*)ptr - m_data) / sizeof(T);
        // size_t idx = ptr - m_data;
        bmap.unset(idx);
    }

    void clear() { bmap.clear(); }
};



