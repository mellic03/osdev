#pragma once
#include <stddef.h>
#include <functional>

namespace ustar
{
    void forEach( void *addr, std::function<void(const char*, void*, size_t)> );
    // void forEach( void *tar, void (*action)( const char *name, void *addr, size_t size ) );

    void list( void *addr );
    // bool find( void *tar, const char *filename, void *&addr, size_t &size );
    void *find( void *addr, const char *filename );
}
