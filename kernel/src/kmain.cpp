#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kpanic.h>
#include <khang.h>
#include <kthread.hpp>

#include <kernel/clock.hpp>
#include <kernel/log.hpp>
#include <kernel/interrupt.hpp>
#include <kernel/syscall.h>
#include <kernel/vfs2.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/boot_limine.hpp>
#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>

#include <string.h>
#include <stdio.h>

#include <driver/mouse.hpp>
#include <driver/pci.hpp>
#include <driver/pit.hpp>
#include <driver/pic.hpp>
#include <driver/apic.hpp>
#include <driver/lapic.hpp>
#include <driver/keyboard.hpp>
#include <driver/serial.hpp>
#include <driver/ramfs.hpp>

#include "syscall/syscall.hpp"
#include "cpu/smp.hpp"



static void genfaultISR( intframe_t* );
static void pagefaultISR( intframe_t* );
static void outOfMemoryISR( intframe_t* );



extern "C"
{
    using ctor_t = void (*)();
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





#include <kassert.h>
#include <math.h>



static void cpu0_main( void* )
{
    // asm volatile ("cli");

    // while (true)
    // {
    //     syslog::kprintf("[cpu0_main]\n");
    // }

    // syslog log("cpu0_main");
    // log("lapic id: %u", info->lapic_id);

    // log("BIG GAY BITCH");

    while (true)
    {
        asm volatile ("hlt");
    }
}



extern void LimineRes_init();
extern void early_init();
extern void late_init();

// external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/plugin/include

static hwDriverInterface *hwDrivers[] = {
    new hwPS2Mouse(),
    new hwPS2Keyboard(),
    new hwdi_PIT(2)
};



extern "C"
void _start()
{
    asm volatile ("cli");

    CPU::createGDT();
    CPU::installGDT();
    CPU::enableSSE();

    PIC::remap(PIC::IRQ_MASTER, PIC::IRQ_SLAVE);
    PIC::disable();
    // enableAPIC();
    // LocalApicInit();

    CPU::createIDT();
    CPU::installIDT();
    CPU::installISR(Int_GENERAL_PROTECTION_FAULT,  genfaultISR);
    CPU::installISR(Int_PAGE_FAULT,                pagefaultISR);
    CPU::installISR(Int_OUT_OF_MEMORY,             outOfMemoryISR);
    CPU::installISR(Int_KTHREAD_START,             ThreadScheduler::trampolineISR);
    CPU::installISR(Int_KTHREAD_YIELD,             ThreadScheduler::scheduleISR);
    CPU::installISR(Int_SYSCALL,                   kernel::syscallISR);

    serial::init();
    syslog log("_start");

    early_init();
    call_ctors();
    late_init();

    // VMM::mapPage(0xDEADBEBE);
    // auto *buf = (uint8_t*)0xDEADBEBE;
    // for (int i=0; i<16; i++)
    //     buf[i] = i*2;
    // for (int i=0; i<16; i++)
    //     log("buf[%d]: %u", i, buf[i]);


    // ramfsEntry *kbraw   = rfs::open("/dev/kb0/raw");
    // ramfsEntry *kbevent = rfs::open("/dev/kb0/event");
    // ramfsEntry *fhsout  = rfs::open("/dev/stdout");
    // rfs::fwrite(kbevent, nullptr, 256);

    vfs2::open<vfs2CharDevice>("/dev/kb0/raw",   256,  nullptr);
    vfs2::open<vfs2CharDevice>("/dev/kb0/event", 256,  nullptr);
    vfs2::open<vfs2CharDevice>("/dev/stdout",    1024, nullptr);
    // vfs2::walk(vfs2::root);

    SMP::init(1);
    kthread::create("idle", kthread::idlemain, nullptr);

    for (auto *hwd: hwDrivers)
    {
        hwd->loadIrqHandler();
        PIC::unmask(hwd->irqno);
    }
    PIC::unmask(2);
    SMP::start();

    for (auto *hwd: hwDrivers)
    {
        if (hwd->entry)
            kthread::create(hwd->name, hwd->entry, hwd->arg);
    }

    // cpu->createThread("SDE",       sde_main, nullptr);
    // cpu->createThread("kshell",    kshell_main, nullptr);
    kthread::create("cpu0_main", cpu0_main, nullptr);
    kthread::start();


    while (true) { asm volatile ("hlt"); }
}
















void stacktrace( intframe_t *frame )
{
    syslog log("stacktrace");
    intframe_t *sf = frame;

    for (int i=0; i<16; i++)
    {
        log("rip: 0x%lx", sf->iret_rip);
        log("rbp: 0x%lx\n", sf->rbp);
        sf = (intframe_t*)(&sf->rbp);
    }
}


static void genfaultISR( intframe_t* )
{
    syslog::kprintf("Exception GENERAL_PROTECTION_FAULT");
    khang();
}

static void outOfMemoryISR( intframe_t* )
{
    syslog::kprintf("Exception OUT_OF_MEMORY");
    khang();
}

static void pagefaultISR( intframe_t *frame )
{
    syslog log("Exception PAGE_FAULT");

    auto *cpu = SMP::this_cpu();
    if (cpu)
        log("cpu %d", cpu->id);

    auto *th = cpu->currThread;
    if (th)
        log("thread %d (%s)", th, th->name);

    log("rip:   0x%lx", frame->iret_rip);
    log("rsp:   0x%lx", frame->iret_rsp);
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








// #include <kernel/elf.h>
// #include <kernel/memory/vmm.hpp>


// void load_phdr( Elf64_Ehdr *ehdr, Elf64_Phdr *phdr )
// {
//     if (phdr->p_type != PT_LOAD)
//         return;

//     VMM::mapPage(phdr->p_paddr, phdr->p_memsz);

//     void* dest = vmm_alloc_at(phdr->p_vaddr, phdr->p_memsz, VM_FLAG_WRITE);
//     memcpy(dest, (void*)ehdr + phdr->p_offset, phdr->p_filesz);

//     const zero_count = phdr->p_memsz - phdr->p_filesz;
//     memset(dest + phdr->p_filesz, 0, zero_count);
// }


// void loop_phdrs( Elf64_Ehdr *ehdr )
// {
//     Elf64_Phdr *phdrs = (Elf64_Phdr*)((uintptr_t)ehdr + ehdr->e_phoff);

//     for (size_t i=0; i<ehdr->e_phnum; i++)
//     {
//         // Elf64_Phdr pheader = phdrs[i];
//         load_phdr(ehdr, &phdrs[i]);
//     }
// }


// void elf_init()
// {
//     ramfsEntry *fh = rfs::open("/bin/test.elf");

//     Elf64_Phdr phdr;

// 	loader_t *elfloader = (loader_t *)malloc(sizeof(loader_t));
// 	elfloader->name = "ELF32";
// 	elfloader->probe = (void *)elf_probe;
// 	elfloader->start = (void *)elf_start;
// 	register_loader(elfloader);
// 	_kill();
// }







