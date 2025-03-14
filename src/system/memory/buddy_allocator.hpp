#pragma once

#include "base_allocator.hpp"
#include "linear_allocator.hpp"

#include "system/data_structures/array.hpp"
#include "system/data_structures/inplace_stack.hpp"


namespace idk
{
    class buddy_allocator
    {
    private:
        static constexpr size_t min_idx  = 7;
        static constexpr size_t max_idx  = 27;
        static constexpr uint64_t BUDDY_MAGIC = 0xA110C;
        // using ptr_type  = void*;
        // using list_type = inplace_stack<void*>[max_idx];
        // list_type *m_freelist;

        using list_type = idk::array<inplace_stack<uintptr_t>, max_idx>;
        list_type *m_freelist;
        uint8_t   *m_data;
        uint8_t   *m_end;

        struct AllocHeader
        {
            uint64_t  magic = BUDDY_MAGIC;
            uintptr_t baseptr;
            void     *usrptr;
            uint64_t  idx;
        };
        
        int    _getidx( size_t );
        void  *_getptr( size_t );


    public:
        buddy_allocator( linear_allocator& );

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
