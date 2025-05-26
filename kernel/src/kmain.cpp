#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kpanic.h>
#include <kthread.hpp>
#include <kprintf.hpp>
#include <new>

#include <kernel/boot_limine.hpp>
#include <kernel/clock.hpp>
#include <kernel/event.hpp>
#include <kernel/log.hpp>
#include <kernel/interrupt.hpp>
#include <kernel/input.hpp>
#include <kernel/syscall.h>
#include <kernel/memory/vmm.hpp>
#include <kernel/module.hpp>
#include <kernel/kvideo.hpp>
#include <kernel/tty.hpp>

#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>
#include "cpu/smp.hpp"

#include <string.h>
#include <stdio.h>

#include <arch/acpi.hpp>
#include <arch/apic.hpp>
#include <driver/pic.hpp>
#include <driver/pci.hpp>
#include <driver/pit.hpp>
#include <driver/serial.hpp>

#include <filesystem/ramfs.hpp>
#include <filesystem/vfs2.hpp>
#include <filesystem/initrd.hpp>

#include "syscall/syscall.hpp"
#include <smp/barrier.hpp>


static void genfaultISR( intframe_t* );
static void pagefaultISR( intframe_t* );
static void outOfMemoryISR( intframe_t* );
static void spuriousISR( intframe_t* );


static void PIT_IrqHandler( intframe_t *frame )
{
    // syslog::println("[PIT_IRQ] cpu=%lu", SMP::this_cpuid());
    auto *cpu = SMP::this_cpu();
    cpu->m_msecs += cpu->m_lapicPeriod;

    ThreadScheduler::scheduleISR(frame);
}


extern void LimineRes_init();
extern void early_init();
static void smp_main(limine_mp_info*);
const auto &println = syslog::println;


__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request lim_rsdp_req = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 3
};






#include <gui/gui.hpp>
static knlTTY kTTY;
static guiTextArea textarea({100, 100}, {250, 250});

static void reeeeeee( void* )
{
    auto *gfxDaemon = (DaemonInterface*)knl::findModule(DaemonModule, DaemonSystem);
    auto *kbdev     = (CharDevInterface*)knl::findModule(DeviceModule, DeviceKeyboard);
    int callback_id = gfxDaemon->listen(kprintf_redraw);
    knl::KbEvent buf[8];

    while (true)
    {
        size_t nbytes = kbdev->read(buf, sizeof(buf));
        size_t count  = nbytes / sizeof(knl::KbEvent);
    
        for (size_t i=0; i<count; i++)
        {
            textarea.putch(buf[i].key);
        }
    }

    gfxDaemon->forget(callback_id);
}


#include <arch/mmio.hpp>

static knl::Barrier smpBarrier{4};
std::atomic_uint64_t smpCount{0};

extern "C"
void _start()
{
    CPU::cli();
    CPU::enableSSE();

    syslog::enable();
    if (!serial::init())
        syslog::disable();
    syslog log("_start");

    early_init();
    kprintf_init(&kTTY, &textarea);
    CPU::createIDT();
    PIC::disable();
    // PCI::init();

    CPU::installISR(IntNo_GEN_FAULT,     genfaultISR);
    CPU::installISR(IntNo_PAGE_FAULT,    pagefaultISR);
    CPU::installISR(IntNo_OUT_OF_MEMORY, outOfMemoryISR);
    CPU::installISR(IntNo_KThreadYield,  ThreadScheduler::scheduleISR);
    CPU::installISR(IntNo_Syscall,       knl::syscallISR);
    CPU::installISR(IntNo_Spurious,      spuriousISR);
    CPU::installIRQ(IrqNo_PIT,           PIT_IrqHandler);

    static ACPI::Response res;
    ACPI::init(lim_rsdp_req.response->address, res);
    APIC::init(res);

    smpBarrier.reset(4);
    SMP::init(smp_main);

    kassert(false); // Should be unreachable!

    CPU::hcf();
}


static void smp_main( limine_mp_info *info )
{
    CPU::cli();
    CPU::enableSSE();

    uint64_t  this_gdt[5];
    gdt_ptr_t this_gdtr;
    CPU::createGDT(this_gdt, &this_gdtr);
    CPU::installGDT(&this_gdtr);

    size_t cpuid = info->lapic_id;
    cpu_t *cpu   = new (SMP::all_cpus + cpuid) cpu_t(cpuid);
    
    while (smpCount.load() != cpu->id)
        asm volatile ("nop");
    CPU::installIDT();
    LAPIC::init();
    smpCount++;

    if (SMP::is_bsp())
    {
        knl::loadModules(initrd::find("drv/"));
        knl::loadModules(initrd::find("srv/"));
        knl::initModules();
        kthread::create("reeeeeee", reeeeeee, nullptr);
    }

    smpBarrier.wait();
    CPU::sti();

    while (true)
    {
        CPU::cli_hlt();
    }
}




void stacktrace( intframe_t *frame )
{
    syslog log("stacktrace");
    intframe_t *sf = frame;

    for (int i=0; i<16; i++)
    {
        log("rip: 0x%lx", sf->rip);
        log("rbp: 0x%lx\n", sf->rbp);
        sf = (intframe_t*)(&sf->rbp);
    }
}


static void genfaultISR( intframe_t* )
{
    syslog log("Exception GENERAL_PROTECTION_FAULT");
    auto *th = SMP::this_thread();
    if (th) log("thread %d (%s)", th, th->name);

    CPU::hcf();
}

static void outOfMemoryISR( intframe_t* )
{
    syslog::print("Exception OUT_OF_MEMORY");
    CPU::hcf();
}

static void spuriousISR( intframe_t* )
{
    return;
}



static std::atomic_int faultCount{0};

static void pagefaultISR( intframe_t *frame )
{
    syslog log("Exception PAGE_FAULT");

    if (faultCount++ > 0)
    {
        kpanic("faultCount++ > 0");
    }

    // auto *cpu = SMP::this_cpu();
    // if (cpu) log("cpu %d", cpu->id);

    auto *th = SMP::this_thread();
    if (th) log("thread %d (%s)", th, th->name);

    log("rip:   0x%lx", frame->rip);
    log("rsp:   0x%lx", frame->rsp);
    log("r11:   0x%lx", frame->r11);
    log("r12:   0x%lx", frame->r12);
    log("r13:   0x%lx", frame->r13);
    log("r14:   0x%lx", frame->r14);
    log("r15:   0x%lx", frame->r15);
    log("rax:   0x%lx", frame->rax);
    log("rbx:   0x%lx", frame->rbx);
    log("rcx:   0x%lx", frame->rcx);
    log("rdx:   0x%lx", frame->rdx);
    log("rdi:   0x%lx", frame->rdi);
    log("rsi:   0x%lx", frame->rsi);
    log("rbp:   0x%lx", frame->rbp);
    log("isrno: %lu", frame->isrno);
    log("errno: %lu", frame->errno);

    uint64_t flags = frame->errno;
    // printBits(flags);

    if (flags & (1<<0))
         log("\t- Page-protection violation");
    else log("\t- Non-present page");

    if (flags & (1<<1))
         log("\t- Caused by write-access");
    else log("\t- Caused by read-access");

    if (flags & (1<<2))
        log("\t- Caused while CPL=3, not necessarily privilege violation");

    if (flags & (1<<3))
        log("\t- One or more page directory entries contain reserved bits which are set to 1. This only applies when the PSE or PAE flags in CR4 are set to 1. ");

    if (flags & (1<<4))
        log("\t- Caused by instruction fetch");

    if (flags & (1<<5))
        log("\t- Caused by a protection-key violation");

    if (flags & (1<<6))
        log("\t- Caused by a shadow stack access");

    if (flags & (1<<15))
        log("\t- Fault was due to an SGX violation. The fault is unrelated to ordinary paging.");
    
    kpanic("Page fault");
}






