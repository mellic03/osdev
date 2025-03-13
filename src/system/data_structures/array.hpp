#pragma once

#include <stdint.h>
#include <stddef.h>


namespace idk
{
    template <typename T, size_t capacity>
    class array;
}



template <typename T, size_t __capacity>
class idk::array
{
private:
    size_t m_cap = __capacity;
    T m_data[__capacity];
    T *m_begin;
    T *m_end;

public:
    array()
    {
        m_begin = m_data;
        m_end   = m_data + m_cap;
    }

    array( const T &value )
    :   array()
    {
        for (size_t i=0; i<m_cap; i++)
        {
            m_data[i] = value;
        }
    }

    size_t size() { return m_cap; }
    size_t capacity() { return m_cap; }
    void *data() { return m_data; }

    T& back () { return m_data[m_cap-1]; }
    const T& back () const { return m_data[m_cap-1]; }

    T& operator[] ( int i ) { return m_data[i]; }
    const T& operator[] ( int i ) const { return m_data[i]; }

};


