#pragma once

#include <stdint.h>
#include <stddef.h>


namespace idk
{
    template <typename T, size_t cap>
    class static_vector;
}


template <typename T, size_t cap>
class idk::static_vector
{
private:
    T  m_base[cap];
    T *m_top;
    T *m_end;

public:

    static_vector()
    :   m_top(m_base),
        m_end(m_base + cap)
    {
        
    }

    static_vector( size_t size )
    :   static_vector()
    {
        m_top = m_base + size;
    }


    static_vector( size_t size, const T &value )
    :   static_vector(size)
    {
        for (T &n: *this)
        {
            n = value;
        }
    }

    // ~static_vector()
    // {
    //     while (!empty())
    //     {
    //         pop_back();
    //     }
    // }

    void push_back( const T &data )
    {
        m_top = ::new (m_top) T(data);
        m_top++;
    }

    void pop_back()
    {
        if (m_top > m_base)
        {
            m_top--;
            m_top->~T();
        }
    }

    // T *find( const T &value )
    // {
    //     for (size_t i=0; i<size(); i++)
    //         if (m_base[i] == value)
    //             return m_base+i;;
    //     return nullptr;
    // }

    T&       front()       { return m_base[0];  }
    const T& front() const { return m_base[0];  }
    T&       back()        { return *(m_top-1); }
    const T& back()  const { return *(m_top-1); }

    T*       data()        { return m_base; }
    const T* data()  const { return m_base; }

    size_t   size()     const { return static_cast<size_t>(m_top - m_base); }
    size_t   capacity() const { return cap; }
    bool     empty()    const { return m_top == m_base; }
    void     clear()          { this->resize(0); }

    void resize( size_t s )
    {
        while (size() > 0) { pop_back();     }
        while (size() < s) { push_back(T()); }
    }


    T &operator[] ( size_t i ) { return *(m_base+i); }
    const T &operator[] ( size_t i ) const { return *(m_base+i); }

    // void forEach( std::function<void(T&)> action )
    // {
    //     for (T &value: *this)
    //         action(value);
    // }

    struct iterator;
    iterator begin() { return iterator(m_base); };
    iterator end()   { return iterator(m_top);  };
};



template <typename T, size_t cap>
struct idk::static_vector<T, cap>::iterator
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



