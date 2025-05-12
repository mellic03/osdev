#pragma once
#include <stdint.h>
#include "ustar.hpp"

namespace initrd
{
    extern uint8_t *tarball;

    // bool find( const char *name, void *&addr, size_t &size );
    void *find( const char *name );
    void init( void *tar );
}

