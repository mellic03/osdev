#pragma once
#include "ustar.hpp"

namespace initrd
{
    extern void *tarball;

    // bool find( const char *name, void *&addr, size_t &size );
    void *find( const char *name );
    void init( void *base );
}

