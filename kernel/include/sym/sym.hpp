#pragma once
#include <kdef.h>
#include "sym_t.hpp"

#include "sym_kernel.hpp"
#include "sym_kthread.hpp"
#include "sym_kvideo.hpp"
#include "sym_libc.hpp"


inline void ksym::loadsym( ksym::ksym_t *sym )
{
    {
        auto &lib = sym->libc_sym;
        std::assert  = lib.assert;
        std::printf  = lib.printf;
        std::malloc  = lib.malloc;
        std::realloc = lib.realloc;
        std::free    = lib.free;
        std::clock   = lib.clock;
    }

    {
        auto &lib = sym->kvideo_sym;
        kvideo::W           = lib.W;
        kvideo::H           = lib.H;
        kvideo::frontbuffer = lib.frontbuffer;
        kvideo::backbuffer  = lib.backbuffer;
        kvideo::rect        = lib.rect;
        kvideo::fill        = lib.fill;
        kvideo::swapBuffers = lib.swapBuffers;
    }

    {
        auto &lib = sym->kthread_sym;
        kthread::yield = lib.yield;
        kthread::sleep = lib.sleep;
        kthread::exit  = lib.exit;
    }

    {
        auto &lib = sym->kernel_sym;
        kernel::panic = lib.panic;
        kernel::hang  = lib.hang;
        kernel::findModule = lib.findModule;
    }
}



