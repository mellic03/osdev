#include "./system.hpp"

#include "drivers/serial.hpp"
#include "gdt/gdt.hpp"
#include "interrupt/interrupt.hpp"


static void __except_bad_alloc()
{
    serial_printf("[__except_bad_alloc] WOOPEE\n");
}

static void __except_bad_free()
{
    serial_printf("[__except_bad_free] WOOPEE\n");
}

static void __except_out_of_memory()
{
    serial_printf("[__except_out_of_memory] WOOPEE\n");
}



int
idk::sys::init( idk::base_allocator *mem )
{
    serial_printf("[idk::sys::init]\n");

    if (!idk_sysio_init(mem))
    {
        return 0;
    }

    idk::sys::loadGDT();
    serial_printf("GDT loaded\n");


    {
        using namespace idk::internal;
        interrupt::initIDT();
        interrupt::registerSystemISR(Exception::BAD_ALLOC,     __except_bad_alloc);
        interrupt::registerSystemISR(Exception::BAD_FREE,      __except_bad_free);
        interrupt::registerSystemISR(Exception::OUT_OF_MEMORY, __except_out_of_memory);
        interrupt::registerSystemISR(Exception::SYSCALL,       __syscall_handler);
        interrupt::loadIDT();
    }
    serial_printf("IDT loaded\n");


    {
        idk::internal::__syscall_init(mem);
    }


    return 1;
}
