// #include <kernel.h>
// #include <kernel/bitmanip.hpp>
// #include <kinterrupt.h>
// #include <algorithm>

// #include "heap_allocator.hpp"
// #include "../driver/serial.hpp"


// // idk::heap_allocator::FreeNode::FreeNode( uint64_t *bs, uint64_t sz )
// // :   base(bs),
// //     size(sz),
// //     next(nullptr)
// // {

// // }



// idk::heap_allocator::heap_allocator( uint64_t *base, uint64_t size )
// {
//     FreeNode *node = (FreeNode*)base;

//     while ((uint64_t*)node < (base + size))
//     {
//         node->size = 1024;
//         node->base = (uint64_t*)node + 3;
//         node->next = node + node->size;
//         node = node->next;
//     }

//     node->size = 0;
//     node->base = (uint64_t*)node + 3;
//     node->next = nullptr;
// }



// void*
// idk::heap_allocator::alloc( size_t nbytes )
// {
//     FreeNode *node = m_freelist;

//     while (node)
//     {
//         if (node->size > nbytes)
//         {
//             break;
//         }

//         node = node->next;
//     }

//     if (!node)
//     {
//         return nullptr;
//     }

//     return (void*)(node->base);

//     // void    *baseptr   = _getptr(idx);
//     // uint8_t *aligned   = ptr_align(unaligned + sizeof(AllocHeader), alignment);
//     // auto    *header    = (AllocHeader*)(aligned - sizeof(AllocHeader));

//     // return (void*)aligned;
// }


// void
// idk::heap_allocator::free( void *usrptr )
// {
//     uint64_t *base  = (uint64_t*)usrptr;
//     FreeNode *unode = (FreeNode*)(base - 3);
//     FreeNode *fnode = m_freelist;

//     while (fnode && fnode->next)
//     {
//         uint64_t lsize = fnode->size;
//         uint64_t usize = unode->size;
//         uint64_t rsize = fnode->next->size;

//         if (lsize <= usize && usize <= rsize)
//         {
//             unode->next = fnode->next;
//             fnode->next = unode;
//             return;
//         }

//         fnode = fnode->next;
//     }

//     // Raise exception
// }


// void
// idk::heap_allocator::clear()
// {
    
// }


