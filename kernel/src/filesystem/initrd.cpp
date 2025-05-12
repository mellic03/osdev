#include <filesystem/initrd.hpp>
#include <kernel/log.hpp>

uint8_t *initrd::tarball;


// bool initrd::find( const char *name, void *&addr, size_t &size )
void *initrd::find( const char *name )
{
    return ustar::find(initrd::tarball, name);
}


void initrd::init( void *tar )
{
    syslog log("initrd::init");
    log("tar: 0x%lx", tar);

    initrd::tarball = (uint8_t*)tar;
    ustar::listAll(initrd::tarball);
}
