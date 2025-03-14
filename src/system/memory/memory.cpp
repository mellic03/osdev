#include "./memory.hpp"
#include "system/drivers/serial.h"


static idk::linear_allocator *__mainblock;
static idk::buddy_allocator  *__buddy_alloc;


void *operator new( size_t nbytes )
{
    return __buddy_alloc->alloc(nbytes, alignof(size_t));
}

void *operator new[]( size_t nbytes )
{
    return __buddy_alloc->alloc(nbytes, alignof(size_t));
}

void operator delete( void *p )
{
    __buddy_alloc->free(p);
}

void operator delete( void *p, size_t )
{
    __buddy_alloc->free(p);
}

void operator delete[]( void *p )
{
    __buddy_alloc->free(p);
}

void operator delete[]( void *p, size_t )
{
    __buddy_alloc->free(p);
}




// idk::base_allocator*
// idk::memory::init( limine_memmap_response *res )
// {
//     uint64_t base = 0x00;
//     uint64_t len  = 0;

//     for (uint64_t i=0; i<res->entry_count; i++)
//     {
//         auto &entry = res->entries[i];

//         if (entry->type == LIMINE_MEMMAP_USABLE)
//         {
//             if (entry->length > len)
//             {
//                 len = entry->length;
//                 base = entry->base;
//             }
//         }
//     }

//     __mainblock = idk::linear_allocator(len, (void*)(0xFFFF800000000000 + base));

//     // auto *base = dynamic_cast<base_allocator*>(&__mainblock);
//     // __buddy_alloc = base->alloc<buddy_allocator>(1);
//     // __buddy_alloc->init(base);

//     return &__mainblock;
// }

int
idk::memory::init( idk::linear_allocator &mainblock, limine_memmap_response *res,
                   uint64_t hhdm )
{
    {
        uint64_t base = 0x00;
        uint64_t len  = 0;

        for (uint64_t i=0; i<res->entry_count; i++)
        {
            auto &entry = res->entries[i];

            if (entry->type == LIMINE_MEMMAP_USABLE)
            {
                if (entry->length > len)
                {
                    len = entry->length;
                    base = entry->base;
                }
            }
        }

        // *mainblock = idk::linear_allocator(len, (void*)(0xffff800000000000 + base));
        mainblock = idk::linear_allocator(len, (void*)(hhdm + base));
    }

    __mainblock   = &mainblock;
    __buddy_alloc = mainblock.alloc<buddy_allocator>(1);
    __buddy_alloc = new (__buddy_alloc) idk::buddy_allocator(mainblock);

    return 1;
}



idk::buddy_allocator&
idk::memory::getBuddy()
{
    return *__buddy_alloc;
}
