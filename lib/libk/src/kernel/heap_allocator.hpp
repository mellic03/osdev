#pragma once

#include <kernel/memory.hpp>


class heap_allocator
{
private:
    size_t    m_cap;
    uintptr_t m_base;
    uintptr_t m_tail;
    uintptr_t m_end;
    std::mutex m_mutex;

public:
    void init( void *base, size_t size );

    size_t bytesAllocated() { return (m_tail - m_base); }
    size_t bytesAvailable() { return (m_end  - m_tail); }

    void *alloc( size_t size, size_t alignment );
    void  free( void* );

};
