#define LIMINE_API_REVISION 3
#include <limine/limine.h>

#include <stdio.h>
#include <kernel/bitmanip.hpp>
#include <kernel/linear_allocator.hpp>
#include <kinplace/inplace_array.hpp>
#include <kinplace/inplace_stack.hpp>

#include "pmm.hpp"
#include "../log/log.hpp"
#include "../ksystem.hpp"


#define PAGE_SIZE 4096

using namespace idk;
uint64_t PMM::hhdm;

uint8_t  *m_bitmap;
uintptr_t m_pbase;
uint64_t  m_npages;




uintptr_t
PMM::alloc()
{
    for (size_t i=0; i<m_npages; i++)
    {
        if (m_bitmap[i] == 0)
        {
            m_bitmap[i] = 1;
            return m_pbase + PAGE_SIZE*i;
        }
    }

    return 0;
}


void page_free( void *ptr )
{
    size_t offset = (uintptr_t)ptr - m_pbase;
    size_t idx    = offset / PAGE_SIZE;
    m_bitmap[idx] = 0;
}




void init_mem( const Krequests &reqs )
{
    // auto *res = reqs.mmaps

}


void
PMM::init( const Krequests &reqs )
{
    syslog log("PMM::init");
    PMM::hhdm = reqs.hhdm;

    uint64_t  phys = 0x100000000;
    uintptr_t base = (uintptr_t)(phys + hhdm);
    size_t    size = 512 * idk::MEGA;
    m_npages = (size / PAGE_SIZE) - 2;

    // linear_allocator mem(base, size);
    // m_bitmap = (uint8_t*)mem.alloc(m_npages, 1);
    // m_pbase  = (uintptr_t)mem.alloc(m_npages*PAGE_SIZE, 1);
    m_bitmap = (uint8_t*)base;
    m_pbase  = (base + m_npages*sizeof(uint8_t));
    m_pbase  = (m_pbase + 4096 - 1) & ~(4096 - 1);

    log("base:    0x%lx", base);
    log("size:    0x%lx", size);
    log("npages:  %lu",   m_npages);
    log("pbase:   0x%lx", m_pbase);

    for (size_t i=0; i<m_npages; i++)
    {
        m_bitmap[i] = 0;
    }
}

/*
    [PMM::init]    
    {    
        base:    0xFFFF800100000000        
        size:    0x20000000        
        npages:  131070        
        pbase:   0x0        
    }
*/

