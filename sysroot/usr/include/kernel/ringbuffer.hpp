#pragma once
#include <stddef.h>
#include <stdint.h>
// #include <mutex>


namespace idk
{
    template <typename T, size_t capacity>
    class RingBuffer;
}


template <typename T, size_t capacity>
class idk::RingBuffer
{
private:
    T data[capacity];
    size_t head = 0;
    size_t tail = 0;
    int    size = 0;
    // std::mutex m_mutex;

public:

    inline void clear()
    {
        head = 0;
        tail = 0;
        size = 0;
    }

	inline bool push_back(const T& item)
	{
        if (size+1 >= capacity)
        {
            return false;
        }

        data[head] = item;
        head = (head+1) % capacity;
        size++;
        return true;
	}

	inline bool pop_front( T& item )
	{
		if (size <= 0)
		{
            return false;
		}

        item = data[tail];
        tail = (tail + 1) % capacity;
        size--;
        return true;
	}
};


