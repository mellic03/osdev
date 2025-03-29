#include <kernel.h>
#include <kernel/bitmanip.hpp>
#include <kinterrupt.h>
#include <algorithm>
#include "heap_allocator.hpp"
#include <kernel/log.hpp>
// #include <kernel/ioport.hpp>


// // idk::heap_allocator::FreeNode::FreeNode( uint64_t *bs, uint64_t sz )
// // :   base(bs),
// //     size(sz),
// //     next(nullptr)
// // {

// // }


idk::heap_allocator::heap_allocator( uintptr_t base, uint64_t size )
{
    m_freelist = (FreeNode*)(base);

    m_freelist->magic = 0xDEADBEBE;
    m_freelist->size = size; // - 2048*sizeof(FreeNode);
    m_freelist->next = nullptr;


    syslog log("heap_allocator::heap_allocator");
    log("m_freelist->size: 0x%lx", m_freelist->size);

}



idk::heap_allocator::FreeNode*
idk::heap_allocator::find( size_t nbytes )
{
    FreeNode *node = m_freelist;

    while (node)
    {
        if (node->size > nbytes)
        {
            return node;
        }

        node = node->next;
    }

    return nullptr;
}


void
idk::heap_allocator::insert( FreeNode *A )
{
    FreeNode *prev = nullptr;
    FreeNode *curr = m_freelist;

    while (curr)
    {
        if (prev && prev->size <= A->size && A->size <= curr->size)
        {
            A->next = curr;
            prev->next = A;
            return;
        }

        prev = curr;
    }
}



void*
idk::heap_allocator::alloc( size_t nbytes )
{
    syslog log("heap_allocator::heap_allocator");
    log("m_freelist->size: 0x%lx", m_freelist->size);

    auto *A = find(nbytes);
    auto Abase = (uintptr_t)(A);

    size_t Asize     = (nbytes + 8-1) & ~(8-1);
    size_t remainder = Asize - A->size;

    FreeNode *B = (FreeNode*)(Abase + sizeof(FreeNode) + Asize);

    B->magic = 0xDEADBEBE;
    B->size = remainder;
    B->next = A->next;

    A->size = Asize;
    A->next = nullptr;

    return (void*)(Abase + sizeof(FreeNode));
}


void
idk::heap_allocator::free( void *ptr )
{
    FreeNode *A = (FreeNode*)((uintptr_t)ptr - sizeof(FreeNode));

    if (A->magic != 0xDEADBEBE)
    {
        KInterrupt<INT_BAD_FREE>();
    }

    insert(A);


    // Raise exception
}


// void
// idk::heap_allocator::clear()
// {
    
// }


