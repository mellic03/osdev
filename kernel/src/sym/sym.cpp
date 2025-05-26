#include <sym/sym_t.hpp>

#include <cpu/cpu.hpp>
#include <kernel/log.hpp>
#include <kernel/clock.hpp>
#include <kernel/kvideo.hpp>
#include <kernel/module.hpp>
#include <kernel/event.hpp>
#include <filesystem/initrd.hpp>
#include <kassert.h>
#include <kmalloc.h>
#include <kthread.hpp>


static ksym::ksym_t kernel_sym;

ksym::ksym_t *ksym::getsym()
{
    {
        auto &lib = kernel_sym.libc_sym;
        lib.assert  = nullptr;
        lib.printf  = syslog::print;
        lib.malloc  = kmalloc;
        lib.realloc = krealloc;
        lib.free    = kfree;
        lib.clock   = kclock::now;
        lib.fopen   = initrd::fopen;
    }

    {
        auto &lib = kernel_sym.video_sym;
        lib.CSR          = &kvideo::CSR;
        lib.W            = kvideo::W;
        lib.H            = kvideo::H;
        lib.frontbuffer  = kvideo::frontbuffer;
        lib.backbuffer   = kvideo::backbuffer;
        // lib.clearBuffer  = kvideo::clearBuffer;
        // lib.fillColor    = kvideo::fillColor;
        // lib.fillBuffer   = kvideo::fillBuffer;
        // lib.rect         = kvideo::rect;
        lib.blit         = kvideo::blit;
        // lib.renderString = kvideo::renderString;
        // lib.cursorString = kvideo::cursorString;
        lib.swapBuffers  = kvideo::swapBuffers;
    }

    {
        auto &lib = kernel_sym.thread_sym;
        lib.create  = kthread::create;
        lib.yield   = kthread::yield;
        lib.sleep   = kthread::sleep;
        lib.exit    = kthread::exit;
        lib.this_cpuid = SMP::this_cpuid;
    }

    {
        auto &lib = kernel_sym.event_sym;
        lib.readMsState  = knl::readMsState;
        lib.writeMsState = knl::writeMsState;
        lib.readMsEvent  = knl::readMsEvent;
        lib.writeMsEvent = knl::writeMsEvent;
        lib.readKbEvent  = knl::readKbEvent;
        lib.writeKbEvent = knl::writeKbEvent;
        lib.listenMsEvent = knl::listenMsEvent;
        lib.listenKbEvent = knl::listenKbEvent;
        lib.emitMsEvent   = knl::emitMsEvent;
        lib.emitKbEvent   = knl::emitKbEvent;
        lib.forgetMsEvent = knl::forgetMsEvent;
        lib.forgetKbEvent = knl::forgetKbEvent;
    }

    {
        // auto &lib = kernel_sym.cpu_sym;
        // lib.installISR = CPU::installISR;
        // lib.installIRQ = CPU::installIRQ;
    }

    {
        auto &lib = kernel_sym.kernel_sym;
        lib.panic = kpanic;
        lib.hcf   = CPU::hcf;
        lib.findModule = knl::findModule;
    }


    return &kernel_sym;
}

