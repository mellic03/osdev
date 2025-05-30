#pragma once
#include <kernel/ringbuffer.hpp>
#include <mutex>

struct vfsNode;

namespace knl
{
    vfsNode *popen( const char *path, size_t stride );
    int pclose( const char *path );

    class Pipe; //: public knl::RingBuffer<char, 512> {  };
}


class knl::Pipe
{
private:
    // uint8_t *m_buf;
    std::mutex m_mutex;
    uint8_t  m_buf[256];
    size_t   m_cap=256;
    size_t   m_size=0;
    size_t   m_head;
    size_t   m_tail;

public:
    size_t m_stride = 2;

    Pipe()
    :   m_size(0), m_head(0), m_tail(0) {  };
    // Pipe( void *buf, size_t size )
    // :   m_buf((uint8_t*)buf), m_cap(size), m_head(0), m_tail(0), m_size(0) {  };

    inline void clear()
    {
        m_head = 0;
        m_tail = 0;
        m_size = 0;
    }

    inline uint8_t &front()
    {
        // kassert(m_size > 0);
        return m_buf[m_head];
    }

    inline uint8_t &back()
    {
        // kassert(m_size > 0);
        return m_buf[(m_tail-1) % m_cap];
    }


	inline void push_back( uint8_t data )
	{
        m_buf[m_tail++] = data;
        m_tail %= m_cap;
        m_size++;
	}

	inline uint8_t pop_front()
	{
        uint8_t data = front();
        m_head = (m_head+1) % m_cap;
        m_size--;
        return data;
	}

    inline size_t read( void *dst, size_t nbytes )
    {
        std::lock_guard lock(m_mutex);

        if (m_size < m_stride)
            return 0;
        if (nbytes > m_cap)
            return 0;

        auto  *dbuf  = (uint8_t*)dst;
        size_t count = 0;

        while (count < nbytes && m_size > 0)
        {
            *(dbuf++) = pop_front();
            count++;
        }

        return count;
    }

    inline size_t write( const void *src, size_t nbytes )
    {
        std::lock_guard lock(m_mutex);

        if (m_size+nbytes > m_cap)
            return 0;

        auto  *sbuf  = (const uint8_t*)src;
        size_t count = 0;

        while (count < nbytes && m_size < m_cap)
        {
            push_back(*(sbuf++));
            count++;
        }

        return count;
    }


    inline size_t size() { return m_size; }
    inline size_t capacity() { return m_cap; }
    inline bool empty() { return m_size==0; }
    inline bool full() { return size() < capacity(); }

    // T &operator[] (int i) { return m_buf[i]; }
};

