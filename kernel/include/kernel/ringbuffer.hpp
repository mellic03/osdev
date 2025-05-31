#pragma once
#include <stddef.h>
#include <stdint.h>
#include <array>
#include <mutex>

// #ifndef kassert
//     #include <kassert.h>
// #endif


namespace knl
{
    template <typename T>
    class ring_buffer;

    template <typename T, size_t N>
    class RingBuffer;
}


template <typename T, size_t N>
class knl::RingBuffer
{
private:
    // lck_t  m_lock;
    T      m_data[N];
    size_t m_head;
    size_t m_tail;
    std::atomic_int m_size{0};

public:

    void clear()
    {
        m_head = 0;
        m_tail = 0;
        m_size.store(0);
    }

    void rotate( int n )
    {
        // std::lock_guard lock(m_lock);
        // kassert(m_size > 0);
        for (int i=0; i<n; i++)
            push_back(pop_front());
    }

    T &front()
    {
        // std::lock_guard lock(m_lock);
        // kassert(m_size > 0);
        return m_data[m_head];
    }

    T &back()
    {
        // std::lock_guard lock(m_lock);
        // kassert(m_size > 0);
        return m_data[(m_tail-1) % N];
    }

	void push_back( const T &item )
	{
        // std::lock_guard lock(m_lock);
        // kassert(m_size+1 < int(N));
        m_data[m_tail++] = item;
        m_tail %= N;
        m_size++;
	}

	T pop_front()
	{
        // std::lock_guard lock(m_lock);
        // kassert(m_size > 0);

        size_t idx = m_head;
        m_head = (m_head+1) % N;
        m_size--;

        return m_data[idx];
	}

    size_t size()     { return m_size.load();            }
    size_t capacity() { return N;                        }
    bool   empty()    { return m_size.load() == 0;       }
    bool   full()     { return m_size.load() >= (int)N;  }

    size_t size()     const { return m_size.load();      }
    size_t capacity() const { return N;                  }
    bool   empty()    const { return m_size.load() == 0; }
    bool   full()     const { return m_size.load() < N;  }

    T &operator[] (int i) { return m_data[i]; }
};









// template <typename T>
// class knl::ring_buffer
// {
// protected:
//     T     *m_buf;
//     size_t m_cap;
//     size_t m_head;
//     size_t m_tail;
//     int    m_size;

// public:
//     ring_buffer()
//     :   m_buf(nullptr), m_cap(0), m_head(0), m_tail(0), m_size(0) {  };

//     ring_buffer( T *buf, size_t cap )
//     :   m_buf(buf), m_cap(cap), m_head(0), m_tail(0), m_size(0) {  };

//     // void init( T *buf, size_t cap )
//     // {
//     //     m_buf = buf;
//     //     m_cap = cap;
//     //     clear();
//     // }

//     void clear()
//     {
//         m_head = 0;
//         m_tail = 0;
//         m_size = 0;
//     }

//     void rotate( int n )
//     {
//         // kassert(m_size > 0);
//         for (int i=0; i<n; i++)
//             push_back(pop_front());
//     }

//     T &front()
//     {
//         // kassert(m_size > 0);
//         return m_buf[m_head];
//     }

//     T &back()
//     {
//         // kassert(m_size > 0);
//         return m_buf[(m_tail-1) % capacity()];
//     }

// 	void push_back( const T &item )
// 	{
//         // kassert(m_size+1 < int(N));
//         m_buf[m_tail++] = item;
//         m_tail %= capacity();
//         m_size++;
// 	}

// 	T pop_front()
// 	{
//         // kassert(m_size > 0);

//         T value = front();
//         m_head = (m_head+1) % capacity();
//         m_size--;

//         return value;
// 	}

//     size_t size() { return m_size; }
//     size_t capacity() { return m_cap; }
//     bool empty() { return m_size==0; }
//     bool full() { return size() < capacity(); }

//     T &operator[](int i) { return m_buf[i]; }
//     const T &operator[](int i) const { return m_buf[i]; }
// };



// template <typename T, size_t N>
// class knl::RingBuffer: public knl::ring_buffer<T>
// {
// private:
//     T m_data[N];

// public:
//     RingBuffer()
//     {
//         this->m_buf = &m_data[0];
//         this->m_cap = N;
//         this->clear();
//         // this->init(&m_data[0], N);
//     };

// };

