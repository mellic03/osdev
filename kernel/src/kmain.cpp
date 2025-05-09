#ifndef __is_kernel
    #define __is_kernel
#endif

#include <kpanic.h>
#include <khang.h>
#include <kthread.hpp>

#include <kernel/boot_limine.hpp>
#include <kernel/clock.hpp>
#include <kernel/log.hpp>
#include <kernel/interrupt.hpp>
#include <kernel/syscall.h>
#include <kernel/memory/vmm.hpp>
#include <kernel/module.hpp>

#include <cpu/cpu.hpp>
#include <cpu/scheduler.hpp>

#include <string.h>
#include <stdio.h>

#include <driver/pci.hpp>
#include <driver/pit.hpp>
#include <driver/pic.hpp>
#include <driver/apic.hpp>
#include <driver/lapic.hpp>
#include <driver/serial.hpp>

#include <filesystem/ramfs.hpp>
#include <filesystem/vfs2.hpp>
#include <filesystem/initrd.hpp>

#include "syscall/syscall.hpp"
#include "cpu/smp.hpp"


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


static void PIT_IrqHandler( intframe_t *frame )
{
    kclock::detail::tick(PIT::MILLISECONDS);
    ThreadScheduler::scheduleISR(frame);
    PIT::reload();
}



static void cpu0_main( void* )
{
    syslog::printf("[cpu0_main]\n");

    // while (true)
    // {
    //     syslog::printf("[cpu0_main]\n");
    // }

    // while (true)
    // {
    //     int values[5];
    //     size_t n = kernel::modules[0].read(values, sizeof(values));
    //     if (n > 0)
    //         syslog::println("[%lu bytes] (x, y): (%d, %d)", n, values[0], values[1]);
    //     kthread::sleep(10);
    // }

    kthread::exit();
}



// external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/plugin/include
extern void early_init();
extern void late_init();


extern "C"
void _start()
{
    asm volatile ("cli");

    CPU::createGDT();
    CPU::installGDT();
    CPU::enableSSE();

    PIC::remap(PIC::IRQ_MASTER, PIC::IRQ_SLAVE);
    PIC::disable();

    PIT::init();
    PIT::set_ms(2);

    // enableAPIC();
    // LocalApicInit();

    CPU::createIDT();
    CPU::installIDT();
    CPU::installISR(IntNo_GENERAL_PROTECTION_FAULT,  genfaultISR);
    CPU::installISR(IntNo_PAGE_FAULT,                pagefaultISR);
    CPU::installISR(IntNo_OUT_OF_MEMORY,             outOfMemoryISR);
    CPU::installISR(IntNo_KTHREAD_START,             ThreadScheduler::trampolineISR);
    CPU::installISR(IntNo_KTHREAD_YIELD,             ThreadScheduler::scheduleISR);
    CPU::installISR(IntNo_SYSCALL,                   kernel::syscallISR);
    CPU::installIRQ(IrqNo_PIT, PIT_IrqHandler);

    serial::init();
    syslog log("_start");

    early_init();
    call_ctors();
    // PCI::init();

    SMP::init(1);
    PIC::unmask(IrqNo_PIT);
    PIC::unmask(2);
    SMP::start();

    kernel::loadModules(initrd::find("modules/"));
    kernel::initModules();

    // cpu->createThread("SDE",       sde_main, nullptr);
    // cpu->createThread("kshell",    kshell_main, nullptr);
    kthread::create("cpu0_main", cpu0_main, nullptr);
    kthread::start();

    kernel::hang();
}





// template <typename ret_type, typename... Args>
// static ret_type someFunctionCall( void *addr, Args... args )
// {
//     using fn_type = ret_type (*)(Args...);
//     return ((fn_type)addr)(args...);
// }



// static void module_load( void *program, size_t size )
// {
//     syslog log("module_load 0x%lx", program);

//     // void *addr = kmalloc(size);
//     // memcpy(addr, program, size);
//     void *addr = program;
//     auto  res  = someFunctionCall<DriverInterface>(addr, &sym_libc);

//     log("name: %s", res.signature);
//     log("type: %u", res.type);
//     log("main: 0x%lx", res.main);
//     deviceDrivers.push_back(res);
// }


// static void elf_test()
// {
//     // syslog log("elf_test");

//     // void  *addr;
//     // size_t size;

//     // if (ustar::find(tarball, "modules/test.bin", addr, size))
//     //     module_load(addr, size);

//     // if (ustar::find(tarball, "modules/mouse.bin", addr, size))
//     //     module_load(addr, size);

//     // Elf64_Ehdr *ehdr = (Elf64_Ehdr*)(fh->sof);
//     // log("e_type:   %s",    Elf64_etypeStr(ehdr->e_type));
//     // log("e_entry:  0x%lx", ehdr->e_entry);
//     // log("e_flags:  %u",    ehdr->e_flags);

//     // Elf64_Phdr *phdrs = (Elf64_Phdr*)(fh->sof + ehdr->e_phoff);
//     // for (int i=0; i<ehdr->e_phnum; i++)
//     // {
//     //     Elf64_Phdr *phdr = phdrs + i;
//     //     log("p_type:   %u",    phdr->p_type);
//     //     log("p_flags:  %u",    phdr->p_flags);
//     //     log("p_offset: %lu",   phdr->p_offset);
//     //     log("p_vaddr:  0x%lx", phdr->p_vaddr);
//     //     log("p_paddr:  0x%lx", phdr->p_paddr);
//     //     log("p_filesz: %lu",   phdr->p_filesz);
//     //     log("p_memsz:  %lu",   phdr->p_memsz);
//     //     log("p_align:  %lu",   phdr->p_align);
//     // }


//     // Elf64_Shdr *shdrs = (Elf64_Shdr*)(fh->sof + ehdr->e_shoff);
//     // for (int i=0; i<ehdr->e_shnum; i++)
//     // {
//     //     Elf64_Shdr *shdr = shdrs + i;

//     //     if (shdr->sh_type == SHT_SYMTAB)
//     //         symtab = shdr;
//     //     else if (shdr->sh_type == SHT_STRTAB && i != ehdr->e_shstrndx)
//     //         strtab = shdr;

//     //     // if (shdr->sh_type == SHT_REL || shdr->sh_type == SHT_RELA)
//     //     // {
//     //     //     log("Relocation section found");
//     //     //     log("sh_addr:   0x%lx",  shdr->sh_addr);
//     //     //     log("sh_offset: 0x%lx",  shdr->sh_offset);
//     //     //     log("sh_size:   %lu",    shdr->sh_size);
//     //     //     log("");
//     //     //     elf_do_shdr(fh->sof, shdr);
//     //     // }
//     // }

// }












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
    syslog::printf("Exception GENERAL_PROTECTION_FAULT");
    kernel::hang();
}

static void outOfMemoryISR( intframe_t* )
{
    syslog::printf("Exception OUT_OF_MEMORY");
    kernel::hang();
}

static void pagefaultISR( intframe_t *frame )
{
    syslog log("Exception PAGE_FAULT");

    if (SMP::is_initialized())
    {
        auto *cpu = SMP::this_cpu();
        if (cpu) log("cpu %d", cpu->id);

        auto *th = cpu->currThread;
        if (th) log("thread %d (%s)", th, th->name);
    }

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






