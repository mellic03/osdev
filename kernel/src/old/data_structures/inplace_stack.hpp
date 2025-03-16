#pragma once

#include <stdint.h>
#include <stddef.h>


namespace idk
{
    template <typename T>
    class inplace_stack;
}



template <typename T>
class idk::inplace_stack
{
private:
    bool _self_alloc = false;
    T *m_start;
    T *m_end;
    T *m_top;

public:
    inplace_stack( T *start, T *end )
    {
        m_start = start;
        m_end   = end;
        m_top   = m_start;
    }

    inplace_stack( T *start, size_t len )
    {
        m_start = start;
        m_end   = start + len;
        m_top   = m_start;
    }

    inplace_stack( size_t len )
    {
        _self_alloc = true;
        m_start = new T[len];
        m_end   = m_start + len;
        m_top   = m_start;
    }

    ~inplace_stack()
    {
        if (_self_alloc)
        {
            delete[] m_start;
        }
    }

    void push( const T &data )
    {
        *m_top = data;
        m_top++;
    }

    void pop()
    {
        m_top--;
    }

    const T &top()
    {
        return *(m_top-1);
    }

    bool empty()       { return (m_top == m_start); }
    bool empty() const { return (m_top == m_start); }

    size_t size()       { return (m_top - m_start); }
    size_t size() const { return (m_top - m_start); }

    void clear() { m_top = m_start; }

};


