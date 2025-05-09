#include <driver/interface.hpp>
#include <sym/sym.hpp>
#include <kmemxx.hpp>

extern "C"
{
    #if UINT32_MAX == UINTPTR_MAX
        #define STACK_CHK_GUARD 0xe2dee396
    #else
        #define STACK_CHK_GUARD 0x595e9fbd94fda766
    #endif
    
    uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
    
    __attribute__((noreturn))
    void __stack_chk_fail(void)
    {
        // kpanic("Stack smashing detected");
        while (true) {  };
    }
}



void driver_init( ksym_t* )
{

}

void driver_close()
{

}

size_t driver_read( void*, size_t )
{
    return 21;
}

size_t driver_write( const void*, size_t  )
{
    return 42;
}



extern "C"
ModuleInterface init( ksym::ksym_t *sym )
{
    // auto *sym = (ksym_t*)ptr;
    // sym->printf("Test from init!\n");
    // printf = sym->printf;

    // auto *interface = (CharDeviceInterface*)sym->malloc(sizeof(CharDeviceInterface));
    // auto *table = (iTableEntry*)sym->malloc(3*sizeof(iTableEntry));

    ModuleInterface dev = {
        .type  = DevInterface_Block,
        .main  = nullptr,
        .init  = driver_init,
        .close = driver_close,
        .read  = driver_read,
        .write = driver_write,
        // .irq   = nullptr,
        // .irqno = 0
    };

    kmemset<char>(dev.signature, '\0', sizeof(dev.signature));
    kmemcpy<char>(dev.signature, "Test", 4);

    return dev;
}

