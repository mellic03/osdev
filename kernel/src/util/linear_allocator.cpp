#include <util/linear_allocator.hpp>
#include <util/bitmanip.hpp>
#include <kernel/log.hpp>
#include <kpanic.hpp>
#include <sys/interrupt.hpp>
#include <algorithm>



knl::linear_allocator::linear_allocator( void *baseptr, size_t nbytes )
{
    // IDK_ASSERT("Capacity must be > 0", nbytes > 0);

    m_capacity = nbytes;
    m_base     = idk::align_up((uintptr_t)baseptr, 16);
    m_tail     = m_base;
    m_end      = m_base + nbytes;

    // m_top.store(reinterpret_cast<size_t>(m_base));

}


knl::linear_allocator::~linear_allocator()
{
    // delete[] m_base;
}


void*
knl::linear_allocator::alloc( size_t nbytes, size_t alignment )
{
    uintptr_t addr = idk::align_up(m_tail, alignment);
    m_tail += nbytes;

    if (m_tail >= m_end)
    {
        kpanic("linear_allocator out of memory");
        return nullptr;
    }

    return (void*)addr;
}


void
knl::linear_allocator::free( void* )
{
    
}


void
knl::linear_allocator::clear()
{
    m_tail = m_base;
}