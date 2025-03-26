#pragma once

#include <stdint.h>
#include <stddef.h>


namespace idk
{
    template <typename T>
    class inplace_array;
}



template <typename T>
class idk::inplace_array
{
private:
    size_t m_cap;
    T *m_data;
    T *m_begin;
    T *m_end;

public:

    inplace_array()
    :   inplace_array(nullptr, 0)
    {

    }

    inplace_array( T *buf, size_t capacity )
    :   m_cap   (capacity),
        m_data  (buf),
        m_begin (buf),
        m_end   (buf+capacity)
    {

    }

    // inplace_array( size_t capacity )
    // {
    //     m_cap   = capacity;
    //     m_data  = new T[capacity];
    //     m_begin = m_data;
    //     m_end   = m_data + m_cap;
    // }

    // inplace_array( size_t capacity, const T &value )
    // :   inplace_array(capacity)
    // {
    //     for (size_t i=0; i<m_cap; i++)
    //     {
    //         m_data[i] = value;
    //     }
    // }

    // ~inplace_array()
    // {
    //     if (self_alloc && m_data)
    //     {
    //         delete[] m_data;
    //     }
    // }


    size_t size() { return m_cap; }
    size_t capacity() { return m_cap; }
    void *data() { return m_data; }

    T& back () { return m_data[m_cap-1]; }
    const T& back () const { return m_data[m_cap-1]; }

    T& operator[] ( int i ) { return m_data[i]; }
    const T& operator[] ( int i ) const { return m_data[i]; }

};


