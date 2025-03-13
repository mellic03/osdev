#pragma once
#include "base_allocator.hpp"


namespace idk
{
    class linear_allocator: public idk::base_allocator
    {
    protected:
        size_t   m_capacity;
        uint8_t *m_base;
        uint8_t *m_tail;
        uint8_t *m_end;

    public:
        linear_allocator() {  };
        linear_allocator( size_t nbytes, base_allocator* );
        linear_allocator( size_t nbytes, void *baseptr );
        ~linear_allocator();

        virtual void *alloc( size_t nbytes, size_t alignment ) override;
        virtual void  free( void* ) override;
        virtual void  clear() override;
    };
}


