#include <kernel/module.hpp>


void driver_open( void* )
{


}

size_t driver_read( void *dst, size_t nbytes )
{
    return nbytes;
}


size_t driver_write( const void *, size_t  )
{
    return 0;
}


static CharDeviceInterface interface;
static iTableEntry table[3];


extern "C"
iTableEntry *init( void* )
{
    interface = CharDeviceInterface(
        driver_open, nullptr, driver_read, driver_write
    );

    table[0] = { ITABLE_BEGIN, NULL };
    table[1] = { ITABLE_CHAR_DEVICE, &interface };
    table[2] = { ITABLE_END, NULL };

    return table;
}

