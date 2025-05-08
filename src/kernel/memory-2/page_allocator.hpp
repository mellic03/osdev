#pragma once

#include <stdint.h>
#include <stddef.h>
#include <kinplace/inplace_array.hpp>


namespace idk
{
    class page_allocator
    {
    private:
        
        // uint8_t  *m_bitmap;
        idk::inplace_array<uint8_t> m_bitmap;
        uint64_t *m_pages;
        size_t    m_psize;

    public:
        page_allocator(): page_allocator(0, 0, 0) {  };
        page_allocator( uint64_t *base, size_t size, size_t page_size );

        void *alloc( size_t nbytes );
        void  free( void* );
        void  clear();

    };

}
