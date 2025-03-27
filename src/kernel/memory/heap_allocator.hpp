#pragma once

#include "memory.hpp"
#include <stdint.h>
#include <stddef.h>


namespace idk
{
    class heap_allocator
    {
    private:
        struct FreeNode
        {
            uint64_t  magic;
            uint64_t  size;
            FreeNode *next;
        
            // FreeNode( uint64_t *bs, uint64_t sz );
        };
    
        // uint64_t *m_base;
        // uint64_t *m_end;
        FreeNode *m_freelist;

        FreeNode *find( size_t nbytes );
        void insert( FreeNode* );

    public:
        heap_allocator(): heap_allocator(0, 0) {  };
        heap_allocator( uintptr_t base, uint64_t size );

        void *alloc( size_t nbytes );
        void  free( void* );
        void  clear();

    };

}
