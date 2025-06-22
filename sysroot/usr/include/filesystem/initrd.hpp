#pragma once
#include <stdint.h>
#include "ustar.hpp"

namespace initrd
{
    extern uint8_t *tarball;

    // bool find( const char *name, void *&addr, size_t &size );
    void *find( const char *name );
    void *fopen( const char *name );
    size_t fsize( void *addr );
    void init( void *tar );
}

