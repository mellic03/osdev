#include <kernel/memory.hpp>
#include "heap_allocator.hpp"
#include <kernel/log.hpp>
#include <kernel/bitmanip.hpp>
#include <sys/interrupt.hpp>
#include <kpanic.h>
#include <algorithm>


struct heap_node
{
    uintptr_t base;
    uintptr_t aligned;
};


void
heap_allocator::init( void *base, size_t size )
{
    syslog log("heap_allocator::heap_allocator");

    size_t block_size = 4*idk::KILO;
    size_t entry_size = sizeof(heap_node) + block_size;

    log("base:      0x%lx", base);
    log("No. nodes: %lu", size/entry_size);

}




void*
heap_allocator::alloc( size_t, size_t )
{
    std::lock_guard lock(m_mutex);

    // void *baseptr  = _getptr(idx);
    // auto  aligned = align_up((uintptr_t)baseptr, alignment);
    // auto *header = (AllocHeader*)aligned;
    // void *usrptr  = (void*)align_up(aligned + sizeof(AllocHeader), alignment);

    // *header = {
    //     .baseptr = (uintptr_t)baseptr,
    //     .idx     = uint64_t(idx)
    // };

    return nullptr;
}


void
heap_allocator::free( void * )
{

}


