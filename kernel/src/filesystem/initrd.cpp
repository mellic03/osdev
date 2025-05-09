#include <filesystem/initrd.hpp>
#include <kernel/log.hpp>

void *initrd::tarball;


// bool initrd::find( const char *name, void *&addr, size_t &size )
void *initrd::find( const char *name )
{
    return ustar::find(initrd::tarball, name);
}


void initrd::init( void *base )
{
    initrd::tarball = base;

    syslog log("initrd::init");
    log("base: 0x%lx", initrd::tarball);
    ustar::list(initrd::tarball);
}
