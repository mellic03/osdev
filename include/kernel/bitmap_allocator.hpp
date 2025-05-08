#pragma once
#include "bitmap.hpp"
#include "bitmanip.hpp"



template <typename T>
class BitMapAllocator
{
private:
    T             *m_data;
    size_t         m_size;
    idk::BitMap2   m_bitmap;

public:

    BitMapAllocator()
    :   m_data(nullptr), m_size(0) {  };

    BitMapAllocator( uint8_t *data, size_t capacity )
    {
        size_t nbytes     = capacity * sizeof(T);
        size_t bitmapSize = capacity / 8;
    
        m_bitmap  = idk::BitMap2(data, bitmapSize);
        m_data    = (T*)idk::align_up(data+bitmapSize, 16);
        m_size    = (data+nbytes) - (uint8_t*)m_data;
    }

    template <typename... Args>
    int32_t alloc( Args... args )
    {
        for (size_t i=0; i<m_bitmap.nbits(); i++)
        {
            if (m_bitmap.is_unset(i))
            {
                m_bitmap.set(i);
                m_data[i] = T(args...);
                return i;
            }
        }
        return -1;
    }

    void free( T *ptr )
    {
        size_t idx = ptr - m_data;
        m_bitmap.unset(idx);
    }

    T *get( size_t idx )
    {
        return &(m_data[idx]);
    }
};


