#include <driver/interface.hpp>
#include <sym/sym.hpp>
#include <kmemxx.hpp>


static uint8_t block_data[1024];

void dev_open()
{

}

void dev_close()
{

}

size_t dev_read( void *dst, size_t offset, size_t nbytes )
{
    kmemcpy<uint8_t>(dst, block_data+offset, nbytes);
    return nbytes;
}

size_t dev_write( const void *src, size_t offset, size_t nbytes )
{
    kmemcpy<uint8_t>(block_data+offset, src, nbytes);
    return nbytes;
}



static BlockDevInterface storagedevice;


extern "C"
ModuleInterface *init( ksym::ksym_t *sym )
{
    ksym::loadsym(sym);

    storagedevice = {
        .modtype  = ModuleType_Device,
        .basetype = DeviceType_Char,
        .subtype  = DeviceType_Mouse,
        .main     = nullptr,

        .open     = dev_open,
        .close    = dev_close,
        .read     = dev_read,
        .write    = dev_write,
        .isrno    = -1,
        .isrfn    = nullptr
    };

    auto &dev = storagedevice;
    kmemset<char>(dev.signature, '\0', sizeof(dev.signature));
    kmemcpy<char>(dev.signature, "Block", 5);

    return (ModuleInterface*)(&dev);
}

