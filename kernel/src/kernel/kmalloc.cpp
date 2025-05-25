
#include <new>
#include <mutex>
#include <kmalloc.h>

#include <kernel/bitmanip.hpp>
#include <kernel/log.hpp>
#include <kinplace/inplace_stack.hpp>
#include <kernel/memory.hpp>
#include <kthread.hpp>


static idk::buddy_allocator *kalloc0;
static std::mutex kmalloc_mutex;


void *kmalloc( size_t size )
{
    std::lock_guard lock(kmalloc_mutex);
    return kalloc0->alloc(size, alignof(max_align_t));
}

void *krealloc( void*, size_t )
{
    std::lock_guard lock(kmalloc_mutex);
    return nullptr;
    // return kalloc0.realloc(ptr, size);
}

void kfree( void *ptr )
{
    std::lock_guard lock(kmalloc_mutex);
    kalloc0->free(ptr);
    // kalloc0.free(((void**)ptr)[-1]);
}




#if 1
    // #include "kmalloc.hpp"
    // static std::mutex kmalloc_mutex;

    void kmalloc_init( uintptr_t base, size_t size )
    {
        using namespace idk;

        syslog log("kmalloc_init");
        linear_allocator kmem0((void*)base, size);

        kalloc0 = (buddy_allocator*)(kmem0.alloc(sizeof(buddy_allocator), 16));
        kalloc0 = new (kalloc0) buddy_allocator(kmem0);
    }
#endif



#ifdef __is_kernel

#endif
