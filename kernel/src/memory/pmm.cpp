#include <kernel/memory/pmm.hpp>
#include <kernel/boot_limine.hpp>
#include <kernel/bitmanip.hpp>
#include <kassert.h>
#include <kmemxx.hpp>
#include <kernel/log.hpp>
#include <kernel/linear_allocator.hpp>
#include <sys/interrupt.hpp>

#include <bitmap.hpp>

#include <kinplace/inplace_array.hpp>
#include <kinplace/inplace_stack.hpp>


uint64_t PMM::hhdm;



struct pmmBitmap
{
    uint8_t *m_data;
    size_t   m_setcount;
    size_t   m_nbits;

    pmmBitmap() {  };

    pmmBitmap( void *base, size_t num_bits )
    {
        num_bits = idk::align_down(num_bits, 8);
        kassert(num_bits % 8 == 0);
        m_data     = (uint8_t*)base;
        m_setcount = 0;
        m_nbits    = num_bits;

        syslog log("pmmBitmap");
        log("m_base:  0x%lx", m_data);
        log("m_end:   0x%lx", m_data + nbytes());
        log("m_nbits: %lu",   m_nbits);

        clear();
    }

    void clear() { kmemset<uint8_t>(m_data, 0, nbytes()); }
    size_t nbytes() { return m_nbits / 8; }
    size_t nbits() { return m_nbits; }

    uint8_t get( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        return (m_data[idx] & (1ULL << bit)) != 0;
    }

    void set( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        m_data[idx] |= (1ULL << bit);
        m_setcount++;
    }

    void unset( size_t i )
    {
        size_t idx = i / 8;
        size_t bit = i % 8;
        m_data[idx] &= ~(1ULL << bit);
        m_setcount--;
    }

    constexpr bool is_set   ( size_t i ) { return  get(i); }
    constexpr bool is_unset ( size_t i ) { return !get(i); }
};



static constexpr size_t PMM_nframes = 512;\
static pmmBitmap PMM_framebmap;
static uintptr_t PMM_framebase;
static uintptr_t PMM_frameend;


static uint64_t  PMM_npages;
static pmmBitmap PMM_pagebmap;
static uintptr_t PMM_pagebase;
static uintptr_t PMM_pageend;


uintptr_t PMM::allocFrame()
{
    static std::mutex M;
    std::lock_guard lock(M);

    for (size_t i=0; i<PMM_framebmap.nbits(); i++)
    {
        if (PMM_framebmap.is_unset(i))
        {
            PMM_framebmap.set(i);
            uintptr_t virt = PMM_framebase + i*PMM::FRAME_SIZE;
            uintptr_t phys = virt - PMM::hhdm;
            kmemset((void*)virt, 0, PMM::FRAME_SIZE);
            // syslog::println("[PMM::allocFrame] setcount=%d", PMM_framebmap.m_setcount);
            return phys;
        }
    }

    return 0;
}


void PMM::freeFrame( uintptr_t phys )
{
    static std::mutex M;
    std::lock_guard lock(M);

    size_t idx = (phys - PMM_framebase - PMM::hhdm) / PMM::FRAME_SIZE;
    PMM_framebmap.unset(idx);
}



uintptr_t PMM::allocPage()
{
    for (size_t i=0; i<PMM_pagebmap.nbits(); i++)
    {
        if (PMM_pagebmap.is_unset(i))
        {
            PMM_pagebmap.set(i);
            uintptr_t virt = PMM_pagebase + PMM::PAGE_SIZE*i;
            uintptr_t phys = virt - PMM::hhdm;
            kmemset((void*)virt, 0, PMM::PAGE_SIZE);
            // syslog::println("[PMM::allocPage] setcount=%d", PMM_pagebmap.m_setcount);
            return phys;
        }
    }

    return 0;
}

static bool isFreeAndContiguous( size_t idx, size_t count )
{
    for (size_t i=0; i<count; i++)
        if (PMM_pagebmap.is_set(idx+i))
            return false;
    return true;
}

uintptr_t PMM::allocPages( size_t n )
{
    for (size_t i=0; i<PMM_pagebmap.nbits(); i++)
    {
        if (isFreeAndContiguous(i, n))
        {
            for (size_t j=0; j>n; j++)
                PMM_pagebmap.set(i+j);

            uintptr_t virt = PMM_pagebase + PMM::PAGE_SIZE*i;
            uintptr_t phys = virt - PMM::hhdm;
            // syslog::println("[PMM::allocPages] setcount=%d", PMM_pagebmap.m_setcount);
            return phys;
        }
    }

    return 0;
}


void PMM::freePage( uintptr_t phys )
{
    size_t idx = (phys - PMM_pagebase - PMM::hhdm) / PAGE_SIZE;
    PMM_pagebmap.unset(idx);
}



void PMM::init( const MemMap &mmap, size_t hhdm_offset )
{
    PMM::hhdm = hhdm_offset;
    syslog log("PMM::init");

    uintptr_t tail = mmap.base + hhdm;
    uintptr_t end  = mmap.base + hhdm + mmap.size;

    PMM_framebmap  = pmmBitmap((void*)tail, PMM_nframes);
    tail          += PMM_framebmap.nbytes();

    PMM_framebase  = idk::align_up(tail, PAGE_SIZE);
    PMM_frameend   = PMM_framebase + PMM_nframes*PMM::FRAME_SIZE;
    tail           = PMM_frameend;
 
    PMM_npages     = (end-tail)/PMM::PAGE_SIZE - 7;
    PMM_pagebmap   = pmmBitmap((void*)tail, PMM_npages);
    tail          += PMM_pagebmap.nbytes();

    PMM_pagebase   = idk::align_up(tail, PAGE_SIZE);
    PMM_pageend    = PMM_pagebase + PMM_npages*PAGE_SIZE;

    PMM_framebmap.clear();
    PMM_pagebmap.clear();

    log("base:      0x%lx", mmap.base + hhdm);
    log("end:       0x%lx", mmap.base + hhdm + mmap.size);
    log("size:      0x%lx", mmap.size);

    log("page size: 0x%lx", PAGE_SIZE);
    log("npages:    %lu",   PMM_npages);

    log("framebase: 0x%lx", PMM_framebase);
    log("frameend:  0x%lx", PMM_frameend);

    log("pagebase:  0x%lx", PMM_pagebase);
    log("pageend:   0x%lx", PMM_pageend);

}

