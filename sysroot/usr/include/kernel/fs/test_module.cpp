#include "module.hpp"
#include <sym/sym.hpp>

// auto clock  = (uint64_t(*)(void))nullptr;
// auto printf = (int(*)(const char*, ...))nullptr;
// auto malloc = (size_t(*)(size_t))nullptr;
// auto memcpy = (size_t(*)(void*, const void*, size_t))nullptr;


int myOpen( void *opt )
{
    return 0;
}

int myClose()
{
    return 0;
}

int myRead( uint64_t addr, size_t nbytes, void *buf )
{
    return 0;
}

int myWrite( uint64_t addr, size_t nbytes, const void *buf )
{
    return 0;
}


// Define the block device interface
static BlockDeviceInterface blockDevice
{ myOpen, myClose, myRead, myWrite };

// Define the interface table
static itable blockInterfaces[]
{
    { BlockDeviceInterface, &blockDevice },
    { END_OF_INTERFACES, NULL }
};


itable *init( symtable_t *sym )
{
    if (sym->type != Sym_Begin)
    {
        return nullptr;
    }

    while (sym->type != Sym_End)
    {
        sym++;
    }

    // Perform initialization logic, if any
    return blockInterfaces;
}

