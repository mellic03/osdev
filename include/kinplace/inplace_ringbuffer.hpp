#pragma once
#include <algorithm>

namespace idk
{
    template <typename T>
    class inplace_ringbuffer;
} // namespace ck



template <typename T>
class idk::inplace_ringbuffer
{
public:


private:
    T  *m_data;
    int m_cap;
    int m_head;
    int m_tail;
    int m_size;

    // void _push( const T &data, int &side )
    // {
    //     m_data[m_tail] = data;
    //     int idx = (m_tail + 1) % m_cap;
    //     m_tail = idx;
    //     m_size += 1;
    // }

    // void _pop( int &side )
    // {
    //     int dir = (side == m_tail) ? -1 : +1;
    //     int idx = (side + dir) % m_cap;
    //     m_data[idx].~T();
    //     side = idx;
    //     m_size -= 1;
    // }

public:
    inplace_ringbuffer( T *data, size_t size )
    {
        m_data = data;
        m_cap  = size;
        m_head = 0;
        m_tail = 0;
        m_size = 0;
    }

    T& front() { return m_data[m_head]; }
    T& back() { return m_data[m_tail]; }

    void push_back( const T &data )
    {
        m_data[m_tail] = data;
        m_tail = (m_tail+1) % m_cap;
        m_size += 1;
    }

    void pop_front()
    {
        m_data[m_head].~T();
        m_head = (m_head+1) % m_cap;
    }

    // void push_front( const T &data ) { _push(data, m_head); }
    // void pop_back() { _pop(m_tail); }

    void *data() { return m_data; }

    size_t capacity() { return m_cap; }
    size_t size() { return m_size; }
    bool empty() { return m_size == 0; }

    size_t bytes_used() { return m_size * sizeof(T); }
    size_t bytes_free() { return capacity() - bytes_used(); }


    bool write( const T *data, size_t count )
    {
        if (bytes_free() < count*sizeof(T))
        {
            return false;
        }

        for (size_t i=0; i<count; i++)
        {
            push_back(data[i]);
        }

        return true;
    }

};