#pragma once

#include "memory.hpp"


namespace idk
{
    class linear_allocator
    {
    private:
        size_t   m_capacity;
        uint8_t *m_base;
        uint8_t *m_tail;
        uint8_t *m_end;

    public:
        linear_allocator() {  };
        linear_allocator( void *baseptr, size_t nbytes );
        ~linear_allocator();
    
        size_t bytesAllocated() { return (m_tail - m_base); }
        size_t bytesRemaining() { return (m_end  - m_tail); }

        void *alloc( size_t nbytes, size_t alignment );
        void  free( void* );
        void  clear();

        template <typename T>
        T *alloc( size_t count = 1 )
        {
            return (T*)(alloc(count*sizeof(T), alignof(T)));
        }

        // template <typename T>
        // T *alloca( size_t count = 1 )
        // {
        //     T *base = alloc<T>(count);
        //     std::memset((void*)base, 0, count*sizeof(T));
        //     return base;
        // }

    };
}
