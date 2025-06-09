// #include <sym/sym_t.hpp>

// #include <cpu/cpu.hpp>
// #include <kernel/log.hpp>
// #include <kernel/clock.hpp>
// #include <kernel/module.hpp>
// #include <kernel/event.hpp>
// #include <ipc/pipe.hpp>
// #include <filesystem/initrd.hpp>
// #include <filesystem/ramfs.hpp>
// #include <filesystem/vfs.hpp>
// #include <kassert.h>
// #include <kmalloc.h>
// #include <kthread.hpp>


// static ksym::ksym_t kernel_sym;

// ksym::ksym_t *ksym::getsym()
// {
//     {
//         auto &lib = kernel_sym.libc_sym;
//         lib.assert  = nullptr;
//         lib.printf  = syslog::print;
//         lib.malloc  = kmalloc;
//         lib.realloc = krealloc;
//         lib.free    = kfree;
//         lib.clock   = kclock::now;
//         lib.fopen   = initrd::fopen;
//     }

//     {
//         auto &lib = kernel_sym.vfs_sym;
//         lib.open  = vfs::open;
//         lib.read  = vfs::read;
//         lib.write = vfs::write;
//         lib.tell  = vfs::tell;
//         lib.seek  = vfs::seek;
//     }

//     {
//         auto &lib = kernel_sym.thread_sym;
//         lib.create  = kthread::create;
//         lib.yield   = kthread::yield;
//         lib.sleep   = kthread::sleep;
//         lib.exit    = kthread::exit;
//         lib.this_cpuid = SMP::this_cpuid;
//     }

//     {
//         // auto &lib = kernel_sym.cpu_sym;
//         // lib.installISR = CPU::installISR;
//         // lib.installIRQ = CPU::installIRQ;
//     }

//     {
//         auto &lib = kernel_sym.kernel_sym;
//         lib.panic = kpanic;
//         lib.hcf   = CPU::hcf;

//         lib.popen  = knl::popen;
//         // lib.fopen  = rfs::open;
//         // lib.fread  = rfs::fread;
//         // lib.fwrite = rfs::fwrite;

//         lib.findModule = knl::findModule;
//     }


//     return &kernel_sym;
// }

