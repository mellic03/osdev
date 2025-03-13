#pragma once
#include "base_allocator.hpp"
#include "system/data_structures/array.hpp"
#include "system/data_structures/inplace_stack.hpp"

namespace idk
{
    class buddy_allocator: public idk::base_allocator
    {
    public:
        static constexpr size_t max_idx  = 10 + 10 + 6;
        static constexpr size_t list_len = 32;
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
            uint64_t  nbytes;
        };
    

        void init( base_allocator* );

        virtual void *alloc( size_t nbytes, size_t alignment ) final;
        virtual void  free( void* ) final;
        virtual void  clear() final;
    };
}
