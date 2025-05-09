#pragma once
#include <kdef.h>
#include "sym_t.hpp"

#include "sym_libc.hpp"
#include "sym_kthread.hpp"

inline void ksym::loadsym( ksym::ksym_t *sym )
{
    {
        auto &lib = sym->libc_sym;
        std::assert  = lib.assert;
        std::printf  = lib.printf;
        std::malloc  = lib.malloc;
        std::realloc = lib.realloc;
        std::free    = lib.free;
    }

    {
        auto &lib = sym->kthread_sym;
        kthread::yield = lib.yield;
        kthread::sleep = lib.sleep;
        kthread::exit  = lib.exit;
    }
}



