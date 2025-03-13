#include "./memory.hpp"

static idk::linear_allocator *__mainblock;
static idk::buddy_allocator  *__buddy_alloc;


// void *operator new( size_t nbytes )
// {
//     return nullptr;
// }

// void operator delete( void *ptr )
// {
    
// }


// idk::linear_allocator*
// idk::memory::init( limine_memmap_response *res )

int
idk::memory::init( idk::linear_allocator *mainblock )
{
    __mainblock = mainblock;

    auto *base = dynamic_cast<base_allocator*>(mainblock);
    __buddy_alloc  = base->alloc<buddy_allocator>(1);
    __buddy_alloc->init(base);

    return 1;
}



idk::buddy_allocator*
idk::memory::getBuddy()
{
    return __buddy_alloc;
}
