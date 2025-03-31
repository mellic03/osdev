// #include "boot/boot.hpp"

// #ifndef __is_kernel
//     #define __is_kernel
// #endif

// #include <kernel/vfs.hpp>
// #include <kernel/bitmanip.hpp>
// #include <kmalloc.h>
// #include <string.h>

// // #include "driver/serial.hpp"
// #include <kernel/log.hpp>

// #include "ksystem.hpp"
// #include "memory/pmm.hpp"
// #include "memory/vmm.hpp"


// using namespace idk;




// KSystem::KSystem( const Krequests &reqs )
// :   m_reqs(reqs)
// // void KSystem::init( Krequests reqs )
// {
//     // syslog log("KSystem::KSystem");
//     // cpu0.init();
    
//     // _load_mmaps();
//     // kmalloc_init(m_mmaps[0]);
//     // PMM::init(m_mmaps[1], reqs.hhdm);
//     // VMM::init();

//     // _load_modules();
//     // KFS::init((uintptr_t)this);
// }


// uint64_t
// KSystem::getHHDM()
// {
//     return m_reqs.hhdm;
// }


// limine_file*
// KSystem::getModule( const char *label )
// {
//     for (auto *file: m_modules)
//     {
//         if (strcmp(file->string, label) == 0)
//         {
//             return file;
//         }
//     }

//     return nullptr;
// }



// void
// KSystem::lsmem()
// {
//     syslog log("KSystem::lsmem");

//     auto *res = m_reqs.mmaps;
//     auto hhdm = getHHDM();

//     // Load usable memmaps
//     for (uint64_t i=0; i<res->entry_count; i++)
//     {
//         auto *e = res->entries[i];

//         log("%s", mmap_str(e->type));
//         log("phys:\t\t0x%lx", e->base);
//         log("base:\t\t0x%lx", hhdm + e->base);
//         log("end: \t\t0x%lx", hhdm + e->base + e->length);
//         log("size:\t\t%luB",  e->length);
//         log("     \t\t%luKB", e->length/idk::KILO);
//         log("     \t\t%luMB", e->length/idk::MEGA);
//     }
// }





// void
// KSystem::_load_modules()
// {
//     syslog log("KSystem::_load_modules");

//     m_modules = inplace_vector<limine_file*>(new limine_file*[64], 64);
//     m_fonts   = inplace_vector<FontBuffer>(new FontBuffer[32], 32);

//     auto *res = m_reqs.modules;

//     for (uint64_t i=0; i<res->module_count; i++)
//     {
//         auto *file = res->modules[i];
//         log("file: \"%s\"", file->string);
//         log("path: \"%s\"", file->path);

//         size_t len = strlen(file->string);
    
//         if (len == 0)
//         {
//             continue;
//         }

//         if (strncmp(file->path, "/font", 5) == 0)
//         {
//             auto *header = (ck_BMP_header*)file->address;
//             m_fonts.push_back(FontBuffer(file->string, header));
//         }

//         m_modules.push_back(file);
//     }

//     log("modules: %d", m_modules.size());

// }







#include "memory/pmm.hpp"
#include "memory/vmm.hpp"

// static u64vec3 thread_args;

// static void thread_execute( void* )
// {
//     uintptr_t entry = thread_args[0];
//     int       argc  = thread_args[1];
//     uint64_t *argv  = (uint64_t*)(thread_args[2]);

//     using YOLO = uint64_t (*)(int, uint64_t*);
//     int64_t result = ((YOLO)entry)(argc, argv);
//     printf("[%s] returned %ld\n", result);
// }



// int
// KSystem::execute( void *address, size_t size, int argc, uint64_t *argv )
// {
//     // syslog log("KSystem::execute");

//     static uintptr_t entry = 0xBEBE0000;
//     static uintptr_t page  = 0;

//     if (page == 0)
//     {
//         page = (uintptr_t)kmalloc(8*PMM::PAGE_SIZE);
//         page = idk::align_up(page, PMM::PAGE_SIZE);
//         VMM::mapRange(page-PMM::hhdm, entry, 7*PMM::PAGE_SIZE);
//     }

//     // log("entry: 0x%lx", entry);
//     memset((void*)entry, 0, 6*PMM::PAGE_SIZE);
//     memcpy((void*)entry, address, size);

//     using YOLO = int (*)(int, uint64_t*);
//     int result = ((YOLO)entry)(argc, argv);

//     // thread_args = u64vec3(
//     //     (uint64_t)entry,
//     //     (uint64_t)argc,
//     //     (uint64_t)argv
//     // );

//     // new kthread(thread_execute, nullptr);

//     return result;
// }
