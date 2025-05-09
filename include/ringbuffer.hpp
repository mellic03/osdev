#pragma once
#include <stddef.h>
#include <stdint.h>
#include <array>


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
    size_t m_head = 0;
    size_t m_tail = 0;
    int    m_size = 0;

public:

    inline size_t size()
    {
        return m_size;
    }

    inline void clear()
    {
        m_head = 0;
        m_tail = 0;
        m_size = 0;
    }

	inline bool push_back(const T& item)
	{
        if (m_size+1 >= capacity)
        {
            return false;
        }

        m_data[m_head] = item;
        m_head = (m_head+1) % capacity;
        m_size++;
        return true;
	}

	inline bool pop_front( T& item )
	{
		if (m_size <= 0)
		{
            return false;
		}

        item = m_data[m_tail];
        m_tail = (m_tail + 1) % capacity;
        m_size--;
        return true;
	}
};


