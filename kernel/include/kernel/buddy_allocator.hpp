#pragma once

#include "memory.hpp"
#include <kernel/array.hpp>
#include <kinplace/inplace_array.hpp>
#include <kinplace/inplace_stack.hpp>
#include <util/linear_allocator.hpp>


namespace idk
{
    class linear_allocator;


    class buddy_allocator
    {
    private:
        static constexpr size_t min_idx  = 7;
        static constexpr size_t max_idx  = 29;
        // static constexpr uint64_t BUDDY_MAGIC = 0xA110C;

        using list_type = idk::array<inplace_stack<uintptr_t>, max_idx>;
        list_type *m_freelist;
        uint8_t   *m_data;
        uint8_t   *m_end;
        std::mutex m_mutex;

        struct AllocHeader
        {
            // uint64_t  magic = BUDDY_MAGIC;
            uintptr_t baseptr;
            uint64_t  idx;
        };
        
        int    _getidx( size_t );
        void  *_getptr( size_t );


    public:
        buddy_allocator();
        buddy_allocator( knl::linear_allocator& );

        void *alloc( size_t nbytes, size_t alignment );
        void  free( void* );
        void  clear();

        template <typename T>
        T *alloc( size_t count = 1 )
        {
            return (T*)(alloc(count*sizeof(T), alignof(T)));
        }

    };

}
