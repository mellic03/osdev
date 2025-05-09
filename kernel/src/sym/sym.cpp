#include <sym/sym_t.hpp>

#include <kernel/log.hpp>
#include <kernel/clock.hpp>
#include <kernel/kvideo.hpp>
#include <kernel/module.hpp>
#include <kassert.h>
#include <kmalloc.h>
#include <khang.h>
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
        lib.clock   = kclock::now;
    }

    {
        auto &lib = kernel_sym.kvideo_sym;
        lib.W           = kvideo::W;
        lib.H           = kvideo::H;
        lib.frontbuffer = kvideo::frontbuffer;
        lib.backbuffer  = kvideo::backbuffer;
        lib.rect        = kvideo::rect;
        lib.fill        = kvideo::fill;
        lib.swapBuffers = kvideo::swapBuffers;
    }

    {
        auto &lib = kernel_sym.kthread_sym;
        lib.yield  = kthread::yield;
        lib.sleep  = kthread::sleep;
        lib.exit   = kthread::exit;
    }

    {
        auto &lib = kernel_sym.kernel_sym;
        lib.panic = kpanic;
        lib.hang  = kernel::hang;
        lib.findModule = kernel::findModule;
    }


    return &kernel_sym;
}

