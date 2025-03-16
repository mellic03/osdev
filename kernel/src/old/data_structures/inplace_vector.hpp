#pragma once

#include <stdint.h>
#include <stddef.h>
#include "memory/memory.hpp"


namespace idk
{
    template <typename T>
    class inplace_vector;
}


template <typename T>
class idk::inplace_vector
{
private:
    size_t m_cap;
    T *m_base;
    T *m_top;
    T *m_end;
    bool self_alloc;

public:

    inplace_vector()
    :   inplace_vector(0, nullptr)
    {
        self_alloc = false;
    }

    inplace_vector( size_t capacity )
    :   inplace_vector(capacity, new T[capacity])
    {
        self_alloc = true;
    }

    inplace_vector( size_t capacity, T *baseptr )
    :   m_cap(capacity),
        m_base(baseptr),
        m_top(baseptr),
        m_end(baseptr+capacity)
    {
        self_alloc = false;
    }

    void init( size_t capacity, T *baseptr )
    {
        m_cap  = capacity;
        m_base = baseptr;
        m_top  = m_base;
        m_end  = m_base + capacity;
        self_alloc = false;
    }

    void deinit()
    {
        while (!empty())
        {
            pop_back();
        }
    }

    // inplace_vector(const inplace_vector &) = delete; // Disable copy constructor
    // inplace_vector &operator=(const inplace_vector &) = delete; // Disable assignment

    // template <typename... Args>
    // void emplace_back( Args&&... args )
    // {
    //     m_top = new (m_top) T(args...);
    //     m_top++;
    // }

    void push_back( const T &data )
    {
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
struct idk::inplace_vector<T>::iterator
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



