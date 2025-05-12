#pragma once

#ifndef MODULE_SYMBOLS
    #define MODULE_SYMBOLS
#endif

#include <kdef.h>
#include "sym_t.hpp"

#include "sym_cpu.hpp"
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
        auto &lib = sym->video_sym;
        kvideo::W           = lib.W;
        kvideo::H           = lib.H;
        kvideo::frontbuffer = lib.frontbuffer;
        kvideo::backbuffer  = lib.backbuffer;
        kvideo::clearBuffer = lib.clearBuffer;
        kvideo::fillColor   = lib.fillColor;
        kvideo::fillBuffer  = lib.fillBuffer;
        kvideo::rect        = lib.rect;
        kvideo::swapBuffers = lib.swapBuffers;
    }

    {
        auto &lib = sym->thread_sym;
        kthread::yield = lib.yield;
        kthread::sleep = lib.sleep;
        kthread::exit  = lib.exit;
    }

    {
        auto &lib = sym->input_sym;
        kinput::triggerMouseEvent = lib.triggerMouseEvent;
        kinput::writeMsData       = lib.writeMsData;
        kinput::readMsData        = lib.readMsData;
    }

    // {
    //     auto &lib = sym->cpu_sym;
    //     CPU::installISR = lib.installISR;
    //     CPU::installIRQ = lib.installIRQ;
    // }

    {
        auto &lib = sym->kernel_sym;
        kernel::panic = lib.panic;
        kernel::hang  = lib.hang;
        kernel::findModule = lib.findModule;
    }
}



