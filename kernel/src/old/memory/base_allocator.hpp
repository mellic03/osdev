#pragma once

#include <stdint.h>
#include <stddef.h>
#include <cstring.hpp>

namespace idk
{
    static constexpr size_t KILO = 1024;
    static constexpr size_t MEGA = 1024*KILO;
    static constexpr size_t GIGA = 1024*MEGA;
};

// namespace idk
// {
//     // template <typename dtype = void>
//     class base_allocator
//     {
//     public:
//         // virtual dtype *alloc( size_t count )
//         // {
//         //     return static_cast<dtype*>(this->alloc(count*sizeof(dtype), alignof(dtype)));
//         // }

//         virtual void *alloc( size_t nbytes, size_t alignment ) = 0;
//         virtual void  free( void* ) = 0;
//         virtual void  clear() = 0;

//         template <typename T>
//         T *alloc( size_t count = 1 )
//         {
//             return (T*)(alloc(count*sizeof(T), alignof(T)));
//         };

//         template <typename T>
//         T *alloca( size_t count = 1 )
//         {
//             T *base = alloc<T>(count);
//             std::memset((void*)base, 0, count*sizeof(T));
//             return base;
//         };
//     };

//     // using base_allocator = templated_allocator<void>;

// }



