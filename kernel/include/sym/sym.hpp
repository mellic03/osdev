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
#include "sym_vfs.hpp"


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
        std::fopen   = lib.fopen;
    }

    {
        auto &lib = sym->vfs_sym;
        uvfs::open  = lib.open;
        uvfs::read  = lib.read;
        uvfs::write = lib.write;
        uvfs::tell  = lib.tell;
        uvfs::seek  = lib.seek;
    }

    {
        auto &lib = sym->video_sym;
        kvideo::CSR          = lib.CSR;
        kvideo::W            = lib.W;
        kvideo::H            = lib.H;
        kvideo::pitch        = lib.pitch;
        kvideo::BPP          = lib.BPP;
        kvideo::frontbuffer  = lib.frontbuffer;
        kvideo::backbuffer   = lib.backbuffer;
        kvideo::swapBuffers  = lib.swapBuffers;
    }

    {
        auto &lib = sym->thread_sym;
        kthread::create = lib.create;
        kthread::yield  = lib.yield;
        kthread::sleep  = lib.sleep;
        kthread::exit   = lib.exit;
        kthread::this_cpuid = lib.this_cpuid;
    }

    // {
    //     auto &lib = sym->cpu_sym;
    //     CPU::installISR = lib.installISR;
    //     CPU::installIRQ = lib.installIRQ;
    // }

    {
        auto &lib = sym->kernel_sym;
        usrknl::panic      = lib.panic;
        usrknl::hcf        = lib.hcf;
        usrknl::popen      = lib.popen;
        usrknl::fopen      = lib.fopen;
        usrknl::fread      = lib.fread;
        usrknl::fwrite     = lib.fwrite;
        usrknl::findModule = lib.findModule;
    }
}



