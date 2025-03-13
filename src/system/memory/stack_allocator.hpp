#pragma once
#include "linear_allocator.hpp"

namespace idk
{
    class stack_allocator: public linear_allocator
    {
    private:
        struct node_type {
            void *tailptr;
            void *usrptr;
        } *m_top;

    public:
        int test_value;

        using linear_allocator::linear_allocator;
        stack_allocator();
        stack_allocator( size_t nbytes, void *baseptr );

        virtual void *alloc( size_t nbytes, size_t alignment ) final;
        virtual void  free( void* ) final;
    };
}


// namespace cock
// {
//     class stack_allocator
//     {
//     private:
//         struct node_type {
//             void *tailptr;
//             void *usrptr;
//         } *m_top;

//         size_t   m_capacity;
//         uint8_t *m_base;
//         uint8_t *m_tail;
//         uint8_t *m_end;

//     public:
//         stack_allocator( size_t nbytes, void *baseptr );

//         void *alloc( size_t nbytes, size_t alignment );
//         void  free( void* );
//     };
// }
