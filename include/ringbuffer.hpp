#pragma once
#include <stddef.h>
#include <stdint.h>
#include <array>


#ifdef __is_kernel
    #include <kassert.h>
    #define SIZE_GTRZERO_CHECK kassert((m_size > 0))
    #define OVERFLOW_CHECK kassert((m_size+1 < int(capacity)))
    #define UNDERFLOW_CHECK kassert((m_size-1 >= 0))
#else
    #define SIZE_GTRZERO_CHECK
    #define OVERFLOW_CHECK
    #define UNDERFLOW_CHECK
#endif



namespace idk
{
    template <typename T, size_t capacity>
    class RingBuffer;
}


template <typename T, size_t capacity>
class idk::RingBuffer
{
private:
    std::array<T, capacity> m_data;
    size_t m_head;
    size_t m_tail;
    int    m_size;

public:

    RingBuffer()
    :   m_head(0), m_tail(0), m_size(0)
    {
        
    }

    inline void clear()
    {
        m_head = 0;
        m_tail = 0;
        m_size = 0;
    }

    inline T &front()
    {
        SIZE_GTRZERO_CHECK;
        return m_data[m_head];
    }

    inline T &back()
    {
        SIZE_GTRZERO_CHECK;
        return m_data[(m_tail-1) % capacity];
    }

	inline bool push_back(const T& item)
	{
        OVERFLOW_CHECK;

        if (size_t(m_size)+1 >= capacity)
            return false;

        m_data[m_tail++] = item;
        m_tail %= capacity;
        m_size++;
        return true;
	}

	inline bool pop_front( T& item )
	{
        UNDERFLOW_CHECK;

		if (m_size <= 0)
            return false;

        item = m_data[m_head++];
        m_head %= capacity;
        m_size--;
        return true;
	}


    inline size_t size() { return m_size; }
    inline bool empty() { return m_size==0; }

};





#undef SIZE_GTRZERO_CHECK
#undef OVERFLOW_CHECK
#undef UNDERFLOW_CHECK
