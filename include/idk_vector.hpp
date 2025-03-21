#pragma once

#include <stdint.h>
#include <stddef.h>
#include <algorithm>
#include <kernel/memory.hpp>


namespace idk
{
    template <typename T>
    class vector;
}


template <typename T>
class idk::vector
{
private:
    size_t m_cap;
    T *m_base;
    T *m_top;
    T *m_end;

    void _realloc( size_t cap )
    {
        size_t sz = size();
        T *bigga = new T[cap];
        if (m_base)
        {
            memcpy(bigga, m_base, sz);
            delete[] m_base;
        }
    
        m_cap  = cap;
        m_base = bigga;
        m_top  = m_base + sz;
        m_end  = m_base + m_cap;
    }


public:
    vector()
    :   m_cap  (8),
        m_base (new T[m_cap]),
        m_top  (m_base + 0),
        m_end  (m_base + m_cap)
    {
        
    }

    vector( size_t size )
    :   m_cap  (std::max(2*size, size_t(8))),
        m_base (new T[m_cap]),
        m_top  (m_base + size),
        m_end  (m_base + m_cap)
    {

    }

    vector( size_t size, const T &value )
    :   vector(size)
    {
        for (size_t i=0; i<size; i++)
        {
            (*this)[i] = value;
        }
    }

    // vector(const vector &) = delete; // Disable copy constructor
    // vector &operator=(const vector &) = delete; // Disable assignment

    // template <typename... Args>
    // void emplace_back( Args&&... args )
    // {
    //     m_top = new (m_top) T(args...);
    //     m_top++;
    // }

    void push_back( const T &data )
    {
        if (m_top+1 == m_end)
        {
            _realloc(2*m_cap);
        }

        m_top = new (m_top) T(data);
        m_top++;
    }

    // void push_back( T &&data )
    // {
    //     emplace_back(data);
    // }

    void pop_back()
    {
        m_top--;
        m_top->~T();
    }


    T&       front()       { return m_base[0];  }
    const T& front() const { return m_base[0];  }
    T&       back()        { return *(m_top-1); }
    const T& back()  const { return *(m_top-1); }

    T*       data()        { return m_base; }
    const T* data()  const { return m_base; }

    size_t   size()     const { return static_cast<size_t>(m_top - m_base); }
    size_t   capacity() const { return m_cap; }
    bool     empty()    const { return m_top == m_base; }
    void     clear()          { this->resize(0); }

    void resize( size_t s )
    {
        while (size() > 0) { pop_back();     }
        while (size() < s) { push_back(T()); }
    }


    T &operator[] ( size_t i ) { return *(m_base+i); }
    const T &operator[] ( size_t i ) const { return *(m_base+i); }

    struct iterator;
    iterator begin() { return iterator(m_base); };
    iterator end()   { return iterator(m_top);  };
};



template <typename T>
struct idk::vector<T>::iterator
{
    T *ptr;

    iterator( T *p )                 : ptr(p)         {  };
    iterator( const iterator &other ): ptr(other.ptr) {  };

    iterator &operator++()
    {
        ptr++;
        return *this;
    };

    iterator operator++(int)
    {
        return iterator(ptr++);
    };

    bool operator == ( const iterator &rhs ) { return ptr == rhs.ptr; };
    bool operator != ( const iterator &rhs ) { return ptr != rhs.ptr; };
    T &operator * () { return *ptr; };

};



