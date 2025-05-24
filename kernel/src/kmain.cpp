#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kassert.h>
#include <kpanic.h>
#include <kthread.hpp>
#include <new>

#include <kernel/boot_limine.hpp>
#include <kernel/clock.hpp>
#include <kernel/log.hpp>
#include <kernel/interrupt.hpp>
#include <kernel/input.hpp>
#include <kernel/syscall.h>
#include <kernel/memory/vmm.hpp>
#include <kernel/module.hpp>
#include <kernel/kvideo.hpp>

#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>
#include <cringe/bmp.hpp>
#include <cringe/font.hpp>

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
#include "smp/smp.hpp"
#include <smp/barrier.hpp>


static void genfaultISR( intframe_t* );
static void pagefaultISR( intframe_t* );
static void outOfMemoryISR( intframe_t* );


extern "C"
{
    using  ctor_t = void(*)();
    extern ctor_t __init_array_start[];
    extern ctor_t __init_array_end[];
}

void call_ctors()
{
    syslog log("call_ctors");
    int i = 0;
    for (ctor_t *ctor = __init_array_start; ctor < __init_array_end; ctor++)
    {
        log("ctor[%d]: 0x%lx", i++, ctor);
        (*ctor)();
    }
}
uint64_t clocksPerSec = 1;

static void PIT_IrqHandler( intframe_t *frame )
{
    {
        static uint64_t prev = 0;

        uint64_t curr = CPU::getTSC();
        uint64_t dTicks = curr - prev;
        uint64_t dTime  = 4; // 1ms
        clocksPerSec = dTicks / dTime;
        prev = curr;
    }

    kclock::detail::tick(PIT::MicroSeconds);
    // syslog::println("[PIT_IrqHandler] CPU %u", SMP::this_cpu()->id);
    ThreadScheduler::scheduleISR(frame);
}


// external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/plugin/include
extern void LimineRes_init();
extern void early_init();
static void smp_main(limine_mp_info*);


__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request lim_rsdp_req = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 3
};


// kbarrier_t barrierA{0};
const auto &println = syslog::println;


extern "C"
void _start()
{
    CPU::cli(); // asm volatile ("cli");
    CPU::enableSSE();

    syslog::enable();
    if (!serial::init())
        syslog::disable();
    syslog log("_start");

    early_init();
    call_ctors();

    ACPI::Response res;
    ACPI::init(lim_rsdp_req.response->address, res);
    APIC::init(res.ioapic_base, 0);

    // barrierA.reset(limine_res.mp->cpu_count);
    // SMP::initMulticore(smp_main);
    SMP::initSinglecore(smp_main);

    kassert(false); // Should be unreachable!

    CPU::hcf();
}


#define CLOCKS_PER_SEC (1000000)

static void thread_test( void* )
{
    auto &msdata = kinput::mousedata;

    while (true)
    {
        // syslog::println("CPS: %lu, time: %lu", clocksPerSec, CPU::getTSC() / clocksPerSec);
        kvideo::renderString("abc ABC", msdata.x.load(), msdata.y.load());
        kthread::yield();
    }
}

static void smp_main( limine_mp_info *info )
{
    CPU::cli(); // asm volatile ("cli");
    CPU::enableSSE();

    // uint64_t  this_gdt[7];
    // gdt_ptr_t this_gdtr;
    // tss_t     this_tss;
    // CPU::createGDT(this_gdt, &this_gdtr, &this_tss);
    // CPU::installGDT(&this_gdtr);
    CPU::createGDT();
    CPU::installGDT();


    size_t cpuid     = info->lapic_id;
    cpu_t *cpu       = new (SMP::all_cpus + cpuid) cpu_t();
           cpu->self = cpu;
           cpu->id   = cpuid;
    CPU::wrmsr(CPU::MSR_GS_BASE, (uint64_t)cpu);
    CPU::wrmsr(CPU::MSR_KERNEL_GS_BASE, (uint64_t)cpu);

    // syslog::println("lapic id: %lu", cpuid);
    // syslog::println("this_gdt: 0x%lx", this_gdt);

    PIC::enable();
    PIC::remap(PIC::IRQ_MASTER, PIC::IRQ_SLAVE);
    PIC::maskAll();
    PIT::init(100);

    CPU::createIDT();
    CPU::installIDT();
    CPU::installISR(IntNo_GENERAL_PROTECTION_FAULT,  genfaultISR);
    CPU::installISR(IntNo_PAGE_FAULT,                pagefaultISR);
    CPU::installISR(IntNo_OUT_OF_MEMORY,             outOfMemoryISR);
    CPU::installISR(IntNo_KTHREAD_YIELD,             ThreadScheduler::scheduleISR);
    CPU::installISR(IntNo_SYSCALL,                   knl::syscallISR);
    CPU::installIRQ(IrqNo_PIT,                       PIT_IrqHandler);

    if (SMP::is_bsp())
    {
        syslog::println("SMP::is_bsp() == true");
        knl::loadModules(initrd::find("drv/"));
        knl::loadModules(initrd::find("srv/"));
        knl::initModules();
    }

    // println("\n\naddr: 0x%lx", addr);
    // println("w, h, bpp: %d, %d, %d", bmp.w, bmp.h, bmp.bpp);
    uint8_t *addr = (uint8_t*)initrd::find("usr/share/font/cutive-w14hf20.bmp");
    BMP_File bmp((void*)(addr + ustar::DATA_OFFSET));
    kvideo::setFont(cringe::Font((uint8_t*)(bmp.data), bmp.w, bmp.h));
    kthread::create("thread_test", thread_test, nullptr);

    PIC::unmask(2);
    PIC::unmask(IrqNo_PIT);
    CPU::sti(); // asm volatile ("sti");

    // CPU::hcf();
    while (true)
    {
        asm volatile ("hlt");
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


static std::atomic_int faultCount{0};

static void pagefaultISR( intframe_t *frame )
{
    syslog log("Exception PAGE_FAULT");

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






