
#include <new>
#include <kmalloc.h>

#include <kernel/bitmanip.hpp>
#include <kernel/log.hpp>
#include <kinplace/inplace_stack.hpp>
#include <kinplace/inplace_vector.hpp>
#include <kernel/memory.hpp>
#include <kthread.hpp>

using namespace idk;
static std::mutex kmalloc_mutex;
static buddy_allocator *kalloc0;

void kmalloc_init( uintptr_t base, size_t size )
{
    syslog log("kmalloc_init");
    linear_allocator kmem0((void*)base, size);

    kalloc0 = (buddy_allocator*)(kmem0.alloc(sizeof(buddy_allocator), 16));
    kalloc0 = new (kalloc0) buddy_allocator(kmem0);
}


void *kmalloc( size_t size )
{
    kthread::yield_guard lock(kthread::lock_count);
    return kalloc0->alloc(size, alignof(max_align_t));
}

// void *kmallocAligned( size_t size, size_t align )
// {
//     uintptr_t p = (uintptr_t)kmalloc(size + 2*align);
//     return (void*)idk::align_up(p, align);
// }

void *krealloc( void*, size_t )
{
    return nullptr;
    // return kalloc0.realloc(ptr, size);
}

void kfree( void *ptr )
{
    kalloc0->free(ptr);
    // kalloc0.free(((void**)ptr)[-1]);
}


#ifdef __is_kernel

#endif
