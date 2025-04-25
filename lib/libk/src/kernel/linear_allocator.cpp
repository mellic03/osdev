#include <kernel/memory.hpp>
#include <kernel/log.hpp>
#include <kernel/bitmanip.hpp>
#include <kpanic.h>
#include <kintcode.h>
#include <kinterrupt.h>
#include <algorithm>



idk::linear_allocator::linear_allocator( void *baseptr, size_t nbytes )
{
    // IDK_ASSERT("Capacity must be > 0", nbytes > 0);

    m_capacity = nbytes;
    m_base     = (uintptr_t)baseptr;
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
    m_tail  = (m_tail + alignment - 1) & ~(alignment - 1);
    m_tail += nbytes;
    m_tail  = (m_tail + alignment - 1) & ~(alignment - 1);

    if (m_tail >= m_end)
    {
        kpanic("linear_allocator out of memory");
        KInterrupt<INT_OUT_OF_MEMORY>();
        return nullptr;
    }

    // syslog log("linear_allocator::alloc(%lu, %u)", nbytes/idk::KILO, alignment);

    // // log("capacity:  %luB",    m_capacity);
    // // log("           %luKB",   m_capacity/idk::KILO);
    // log("capacity:  %luMB",   m_capacity/idk::MEGA);

    // // log("used:      %luB",    (m_tail-m_base));
    // log("used:      %luKB",   (m_tail-m_base)/idk::KILO);
    // log("           %luMB",   (m_tail-m_base)/idk::MEGA);

    // // log("remaining: %luB",      m_capacity - m_tail);
    // log("remaining: %luKB",    (m_end - m_tail) / idk::KILO);
    // log("           %luMB",    (m_end - m_tail) / idk::MEGA);

    // log("base:      0x%lx",   m_base);
    // log("end:       0x%lx",   m_end);
    // log("sz:        %lu",     m_end-m_tail);
    // log("tail:      0x%lx",   m_tail);

    if (m_tail >= m_end)
    {
        kpanic("linear_allocator out of memory");
        KInterrupt<INT_OUT_OF_MEMORY>();
    }
    // LOG_ASSERT(
    //     m_tail < m_end,
    //     "Out of memory ({} / {} bytes)",
    //     static_cast<size_t>(m_tail-m_base),
    //     static_cast<size_t>(m_end-m_base)
    // );

    return reinterpret_cast<void*>(m_tail - nbytes);
}


void
idk::linear_allocator::free( void* )
{
    
}


void
idk::linear_allocator::clear()
{
    m_tail = m_base;
}