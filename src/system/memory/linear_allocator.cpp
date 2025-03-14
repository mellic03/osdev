#include "linear_allocator.hpp"
#include "bitmanip.hpp"
#include "system/interrupt/interrupt.hpp"



idk::linear_allocator::linear_allocator( size_t nbytes, void *baseptr )
{
    // IDK_ASSERT("Capacity must be > 0", nbytes > 0);

    m_capacity = nbytes;
    m_base     = (uint8_t*)baseptr;
    m_tail     = m_base;
    m_end      = m_base + nbytes;

    // m_top.store(reinterpret_cast<size_t>(m_base));

}


idk::linear_allocator::~linear_allocator()
{
    // delete[] m_base;
}


void*
idk::linear_allocator::alloc( size_t nbytes, size_t alignment )
{
    // uint8_t *top = reinterpret_cast<uint8_t*>(m_top.fetch_add(nbytes));
    //          top = align_ptr(top, alignment);
    // return static_cast<void*>(top);

    m_tail = idk::ptr_align(m_tail, alignment) + nbytes;

    if (m_tail >= m_end)
    {
        idk::Interrupt(Exception::OUT_OF_MEMORY);
        return nullptr;
    }

    // LOG_ASSERT(
    //     m_tail < m_end,
    //     "Out of memory ({} / {} bytes)",
    //     static_cast<size_t>(m_tail-m_base),
    //     static_cast<size_t>(m_end-m_base)
    // );

    return static_cast<void*>(m_tail - nbytes);
}


void
idk::linear_allocator::free( void *ptr )
{
    uint64_t addr = (uint64_t)(ptr);
    addr++;
}


void
idk::linear_allocator::clear()
{
    m_tail = m_base;
}