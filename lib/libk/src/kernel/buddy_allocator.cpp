#include <kernel/memory.hpp>
#include <kernel/log.hpp>
#include <kernel/bitmanip.hpp>
#include <kintcode.h>
#include <kinterrupt.h>
#include <string.h>
#include <algorithm>

// size_t size_table[3][10] = {
// //     1    2    4    8    16   32  64   128  256  512
//     {  0,   0,   0,   0,   0,   0,   0,  64,  64,  64 }, // B
//     { 32,  32,  32,  16,  16,  16,  16,   8,   8,   8 }, // KB
//     {  8,   8,   8,   8,   4,   4,   2,   2,   0,   0 }, // MB
// };

// size_t m_size = 0;
// for (int i=0; i<10; i++)
// {
//     m_size += size_table[0][i] * (1<<i);
//     m_size += size_table[1][i] * (1<<i) * idk::KILO;
//     m_size += size_table[2][i] * (1<<i) * idk::MEGA;
// }

idk::buddy_allocator::buddy_allocator()
:   m_freelist (nullptr),
    m_data     (nullptr),
    m_end      (nullptr)
{

}


idk::buddy_allocator::buddy_allocator( idk::linear_allocator &A )
{
    syslog log("buddy_allocator::buddy_allocator");

    // uint8_t  m_freelist[
    // B 1 2 4 8 16 32 64 128 256 512
    // K 1 2 4 8 16 32 64 128 256 512
    // M 1 2 4 8 16 32 // Support allocations of up to 32MB

    // - Want to allocate 9KB
    // - Round up to 16KB == 16384B = 2^14
    // - Take free pointer from m_freelist[14]

    size_t size_table[3*10] = {
    //  1     2     4    8    16   32   64    128   256   512
        0,    0,    0,   0,   0,   0,   256,  256,  256,  256, // B
        256,  256,  64,  32,  32,  16,  16,   16,   32,   32, // KB
        16,   16,   16,  16,  4,   4,   2,    2,    1,    0  // MB
    }; 

    size_t m_size = 0;
    for (size_t i=0; i<30; i++)
    {
        m_size += (1<<i) * size_table[i];
        // m_size += size_table[0*10 + i] * (1<<i);
        // m_size += size_table[1*10 + i] * (1<<i) * idk::KILO;
        // m_size += size_table[2*10 + i] * (1<<i) * idk::MEGA;
    }

    m_freelist = A.alloc<list_type>(1);
    m_data     = A.alloc<uint8_t>(m_size);
    m_end      = m_data + m_size;


    uintptr_t tail = (uintptr_t)m_data;

    for (size_t i=min_idx; i<=max_idx; i++)
    {
        size_t block_nbytes = (1<<i);
        size_t block_count  = size_table[i];

        auto &stack = (*m_freelist)[i];
        stack = inplace_stack<uintptr_t>(block_count, A.alloc<uintptr_t>(block_count));

        for (size_t j=0; j<block_count; j++)
        {
            stack.push(tail);
            tail += block_nbytes;
        }
    }

    log("m_size: %luB",  m_size);
    log("        %luKB", m_size/idk::KILO);
    log("        %luMB", m_size/idk::MEGA);
    log("base:   0x%lx", m_data);
    log("end:    0x%lx", m_end);
    log("sz:     %lu",   m_end-m_data);
    log("tail:   0x%lx", tail);

}


int
idk::buddy_allocator::_getidx( size_t nbytes )
{
    int idx = (int)(bit_index(nearest_pow2(nbytes)) + 1);
        idx = std::clamp(idx, int(min_idx), int(max_idx));
    auto *stack = &((*m_freelist)[idx]);

    while (stack->empty() && idx < int(max_idx))
    {
        idx += 1;
        stack = &((*m_freelist)[idx]);
    }

    if (stack->empty())
    {
        return -1;
    }

    return idx;
}


uintptr_t
idk::buddy_allocator::_getptr( size_t idx )
{
    auto &stack = (*m_freelist)[idx];

    uintptr_t ptr = stack.top();
                    stack.pop();

    return ptr;
}


void*
idk::buddy_allocator::alloc( size_t nbytes )
{
    int idx = _getidx(nbytes);

    if (idx == -1)
    {
        KInterrupt<INT_OUT_OF_MEMORY>();
        return nullptr;
    }

    uintptr_t baseptr = _getptr(idx);
    uintptr_t aligned = align_up(baseptr + sizeof(AllocHeader), alignof(max_align_t));
    auto     *header  = (AllocHeader*)(aligned - sizeof(AllocHeader));

    *header = {
        .baseptr = (uintptr_t)baseptr,
        .idx     = uint64_t(idx)
    };

    return (void*)aligned;
}

void*
idk::buddy_allocator::realloc( void *ptr, size_t newsize )
{
    uintptr_t aligned  = (uintptr_t)ptr;
    auto     *header   = (AllocHeader*)(aligned - sizeof(AllocHeader));
    size_t    prevsize = (size_t(1) << header->idx);

    void *bigga = this->alloc(newsize);
    memcpy(bigga, ptr, prevsize);
    this->free(ptr);

    return bigga;
}


void
idk::buddy_allocator::free( void *ptr )
{
    uintptr_t aligned = (uintptr_t)ptr;
    auto     *header  = (AllocHeader*)(aligned - sizeof(AllocHeader));

    // Sanity checks
    // -----------------------------------------------------------------------------------------
    // if (header->magic != BUDDY_MAGIC)
    // {
    //     SYSLOG("[buddy_allocator::free] header->magic != BUDDY_MAGIC");
    //     idk::Interrupt(Exception::BAD_FREE);
    // }

    // if (header->usrptr != usrptr)
    // {
    //     idk::Interrupt(Exception::BAD_FREE);
    // }

    // size_t idx = bit_index(nearest_pow2(nbytes)) + 1;
    // if (idx != bit_index(nearest_pow2(nbytes)))
    // {
    //     idk::Interrupt(Exception::BAD_FREE);
    // }
    // -----------------------------------------------------------------------------------------


    auto &stack = (*m_freelist)[header->idx];
    stack.push(header->baseptr);
    // if (ptr != m_top->usrptr)
    // {
    //     // LOG_INFO("usrptr={} m_top->usrptr={}", ptr, m_top->usrptr);
    //     // LOG_FATAL("Memory must be freed in LIFO order");
    // }

    // m_tail = static_cast<uint8_t*>(m_top->tailptr);
}


void
idk::buddy_allocator::clear()
{
    
}


