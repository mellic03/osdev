#include <kernel/memory/pmm.hpp>

#include <kernel/bitmanip.hpp>
#include <kernel/bitmap.hpp>
#include <kernel/linear_allocator.hpp>
#include <kernel/log.hpp>
#include <kmemxx.hpp>

#include <kinplace/inplace_array.hpp>
#include <kinplace/inplace_stack.hpp>




using namespace idk;
uint64_t PMM::hhdm;

uintptr_t m_bitmap_end;
uintptr_t m_pbase;
uintptr_t m_pend;
uint64_t  m_npages;


uint64_t *m_bitmap;

static inline
uint8_t get_bit(size_t i)
{
    size_t idx = i / (sizeof(uint64_t) * 8);
    size_t bit = i % (sizeof(uint64_t) * 8);
    return (m_bitmap[idx] & (1ULL << bit)) ? 1 : 0;
}

static inline
void set_bit(size_t i)
{
    size_t idx = i / (sizeof(uint64_t) * 8);
    size_t bit = i % (sizeof(uint64_t) * 8);
    m_bitmap[idx] |= (1ULL << bit);
}

static inline
void unset_bit(size_t i)
{
    size_t idx = i / (sizeof(uint64_t) * 8);
    size_t bit = i % (sizeof(uint64_t) * 8);
    m_bitmap[idx] &= ~(1ULL << bit);
}



uintptr_t PMM::alloc()
{
    for (size_t i=0; i<m_npages; i++)
    {
        if (get_bit(i) == 0)
        {
            set_bit(i);

            uintptr_t virt = m_pbase + PAGE_SIZE*i;
            uintptr_t phys = virt - hhdm;
            kmemset<uint128_t>((void*)virt, 0, PAGE_SIZE);
            return phys;
        }
    }

    return 0;
}


void PMM::free( uintptr_t phys )
{
    size_t idx = (phys - m_pbase - hhdm) / PAGE_SIZE;
    unset_bit(idx);
}



void PMM::init( const MemMap &mmap, size_t hhdm_offset )
{
    PMM::hhdm = hhdm_offset;
    syslog log("PMM::init");

    m_npages     = (mmap.size / PAGE_SIZE) - 2;
    m_bitmap     = (uint64_t*)(mmap.base + hhdm);
    m_bitmap_end = (uintptr_t)m_bitmap + m_npages * (sizeof(uint64_t)/8);
    m_pbase      = align_up(m_bitmap_end, PAGE_SIZE);
    m_pend       = m_pbase + m_npages;

    log("base:       0x%lx", mmap.base + hhdm);
    log("size:       0x%lx", mmap.size);
    log("bitmap:     0x%lx", m_bitmap);
    log("bitmap_end: 0x%lx", m_bitmap_end);

    log("page size:  0x%lx", PAGE_SIZE);
    log("npages:     %lu",   m_npages);
    log("pbase:      0x%lx", m_pbase);
    log("pend:       0x%lx", m_pend);

    for (size_t i=0; i<m_npages; i++)
    {
        m_bitmap[i] = 0;
    }
}



