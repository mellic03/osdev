#include <sym/sym_type.hpp>

#include <kernel/log.hpp>
#include <kernel/clock.hpp>
#include <kassert.h>
#include <kmalloc.h>
#include <kthread.hpp>


static ksym::ksym_t kernel_sym;

ksym::ksym_t *ksym::getsym()
{
    {
        auto &lib = kernel_sym.libc_sym;
        lib.assert  = nullptr;
        lib.printf  = syslog::printf;
        lib.malloc  = kmalloc;
        lib.realloc = krealloc;
        lib.free    = kfree;
    }

    {
        auto &lib = kernel_sym.kthread_sym;
        lib.yield  = kthread::yield;
        lib.sleep  = kthread::sleep;
        lib.exit   = kthread::exit;
    }


    return &kernel_sym;
}

