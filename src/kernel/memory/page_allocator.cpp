#include <kernel.h>
#include <kernel/bitmanip.hpp>
#include <kinterrupt.h>
#include <algorithm>

#include "page_allocator.hpp"
#include <kernel/ioport.hpp>



idk::page_allocator::page_allocator( uint64_t *base, size_t size, size_t page_size )
{
    size_t page_count = size / page_size;
    size_t mapsize    = page_count * sizeof(uint8_t);

    uint8_t *A = (uint8_t*)base;
    // uint8_t *B = (uint8_t*)base + mapsize;
    m_bitmap = inplace_array<uint8_t>(A, mapsize-1);
    m_pages  = base + page_size;
    m_psize  = page_size;

    for (size_t i=0; i<m_bitmap.size(); i++)
    {
        m_bitmap[i] = 0;
    }
}



void*
idk::page_allocator::alloc( size_t )
{
    for (size_t i=0; i<m_bitmap.size(); i++)
    {
        if (m_bitmap[i] == 0)
        {
            m_bitmap[i] = 1;
            return (void*)(m_pages[i]);
        }
    }

    return nullptr;
}


void
idk::page_allocator::free( void *usrptr )
{
    if (usrptr)
    {
        
    }
}


void
idk::page_allocator::clear()
{
    
}


