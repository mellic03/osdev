#pragma once
#include <algorithm.hpp>

namespace ck
{
    template <typename T>
    class RingBuffer;
} // namespace ck



template <typename T>
class ck::RingBuffer
{
public:


private:
    T  *m_data;
    int m_len;
    int m_head;
    int m_tail;
    int m_size;


public:
    RingBuffer( int len, T *data )
    {
        m_data = data;
        m_len  = len;
        m_head = 0;
        m_tail = 1;
        m_size = 0;
    }

    size_t size()
    {
        return m_size;
    }

    void push_back( const T &data )
    {
        m_data[m_tail] = data;
        m_tail = (m_tail+1) % m_len;
        m_size += 1;
    }

    void pop_back( const T &data )
    {
        (m_tail-1).~T();
        m_tail = (m_tail-1) % m_len;
        m_size -= 1;
    }

    void push_front( const T &data )
    {
        *m_head = data;
        m_head = (m_head-1) % m_len;
        m_size += 1;
    }

    void pop_front( const T &data )
    {
        (m_head+1).~T();
        m_head = (m_head-1) % m_len;
        m_size -= 1;
    }

};