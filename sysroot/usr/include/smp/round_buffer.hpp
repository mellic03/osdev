#pragma once
#include <stddef.h>
#include <stdint.h>
#include <kassert.h>
#include <array>

namespace idk
{
    template <typename T, size_t N>
    class RoundBuffer;
}


template <typename T, size_t N>
class idk::RoundBuffer
{
private:
    std::array<T, N> m_data;
    size_t m_head;
    size_t m_tail;
    int    m_size;

public:

    inline void clear()
    {
        m_head = 0;
        m_tail = 0;
        m_size = 0;
    }

    inline void rotate( int n )
    {
        kassert(m_size > 0);
        for (int i=0; i<n; i++)
            push_back(pop_front());
    }

    inline T &front()
    {
        kassert(m_size > 0);
        return m_data[m_head];
    }

    inline T &back()
    {
        kassert(m_size > 0);
        return m_data[(m_tail-1) % N];
    }

	inline void push_back( const T& item )
	{
        kassert(m_size+1 < int(N));
        m_data[m_tail++] = item;
        m_tail %= N;
        m_size++;
	}

	inline T pop_front()
	{
        kassert(m_size > 0);

        T value = front();
        m_head = (m_head+1) % N;
        m_size--;

        return value;
	}


    inline size_t size() { return m_size; }
    inline bool empty() { return m_size==0; }

};