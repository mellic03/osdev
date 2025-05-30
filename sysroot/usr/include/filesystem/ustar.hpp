#pragma once
#include <stddef.h>
// #include <functional>

namespace ustar
{
    static constexpr size_t NAME_OFFSET  = 0;
    static constexpr size_t MAGIC_OFFSET = 257;
    static constexpr size_t DATA_OFFSET  = 512;

    void forEach( void *tar, void(*)(void*, size_t) );
    // void listChilren( void *tar );
    void listAll( void *tar );
    void *find( void *tar, const char *filename );
}
