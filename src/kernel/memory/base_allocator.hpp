#pragma once

#include <stdint.h>
#include <stddef.h>


namespace ck
{
    // template <typename dtype = void>
    class base_allocator
    {
    public:
        // virtual dtype *alloc( size_t count )
        // {
        //     return static_cast<dtype*>(this->alloc(count*sizeof(dtype), alignof(dtype)));
        // }

        virtual void *alloc( size_t nbytes, size_t alignment ) = 0;
        virtual void  free( void* ) = 0;
        virtual void  clear() = 0;
    
    };

    // using base_allocator = templated_allocator<void>;

}



