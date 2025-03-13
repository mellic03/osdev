#include "buddy_allocator.hpp"
#include "bitmanip.hpp"
#include "system/interrupt/interrupt.hpp"
#include "system/drivers/serial.h"


struct FreeList
{
    void *ptrs[512];

    void init( size_t block_size, idk::base_allocator *A )
    {
        for (int i=0; i<512; i++)
        {
            ptrs[i] = A->alloc(block_size, 1);
        }
    }
};


void
idk::buddy_allocator::init( base_allocator *A )
{
    // uint8_t  m_freelist[
    // B 1 2 4 8 16 32 64 128 256 512
    // K 1 2 4 8 16 32 64 128 256 512
    // M 1 2 4 8 16 32 // Support allocations of up to 32MB

    // - Want to allocate 9KB
    // - Round up to 16KB == 16384B = 2^14
    // - Take free pointer from m_freelist[14]
    m_freelist = A->alloc<list_type>(1);
    m_data     = A->alloc<uint8_t>(128*idk::MEGA);
    m_end      = m_data + 128*idk::MEGA;


    uint8_t *tail = m_data;

    for (size_t i=0; i<max_idx; i++)
    {
        auto &stack = (*m_freelist)[i];

        stack = inplace_stack<uintptr_t>(
            A->alloc<uintptr_t>(list_len),
            list_len
        );

        size_t block_nbytes = (1<<i);

        for (size_t j=0; j<list_len; j++)
        {
            // uint8_t *ptr = A->alloc<uint8_t>(block_nbytes);
            void *ptr = (void*)tail;
            stack.push((uintptr_t)ptr);

            tail += block_nbytes;
        }
    }

}


void*
idk::buddy_allocator::alloc( size_t nbytes, size_t alignment )
{
    size_t idx = bit_index(nearest_pow2(nbytes)) + 1;
    auto &stack = (*m_freelist)[idx];

    if (stack.empty())
    {
        idk::Interrupt(Exception::OUT_OF_MEMORY);
        return nullptr;
    }

    void    *baseptr   = (void*)(stack.top()); stack.pop();
    uint8_t *unaligned = static_cast<uint8_t*>(baseptr);
    uint8_t *aligned   = ptr_align(unaligned + sizeof(AllocHeader), alignment);
    auto    *header    = (AllocHeader*)(aligned - sizeof(AllocHeader));

    *header = {
        .magic   = BUDDY_MAGIC,
        .baseptr = (uintptr_t)baseptr,
        .usrptr  = aligned,
        .idx     = idx,
        .nbytes  = nbytes
    };


    serial_printf(
        "[buddy_allocator::alloc] nbytes=%u, idx=%u, 2^idx==%u\n",
        nbytes, idx, (1<<idx)
    );
    serial_printf("[buddy_allocator::free] magic=0x%x\n", header->magic);

    return (void*)aligned;
}


void
idk::buddy_allocator::free( void *usrptr )
{
    uint8_t *aligned = (uint8_t*)usrptr;
    auto    *header  = (AllocHeader*)(aligned - sizeof(AllocHeader));

    serial_printf("[buddy_allocator::free] magic=0x%x\n", header->magic);
    return;

    // Sanity checks
    // -----------------------------------------------------------------------------------------
    if (header->magic != BUDDY_MAGIC)
    {
        idk::Interrupt(Exception::BAD_FREE);
    }

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


