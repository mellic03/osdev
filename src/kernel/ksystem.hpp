#pragma once

#include <stddef.h>
#include <stdint.h>

#include "cpu/cpu.hpp"
#include "tty.hpp"
#include "process/process.hpp"
#include "video/video.hpp"
#include <kernel/memory.hpp>
#include <kinplace/inplace_vector.hpp>


struct limine_framebuffer_response;
struct limine_executable_address_response;
struct limine_module_response;
struct limine_memmap_response;
struct limine_mp_response;
struct limine_file;


namespace idk
{
    struct ExecHeader
    {
        void *addr;
        uint64_t size;
    };
        
    struct Krequests
    {
        uint64_t                      hhdm;
        limine_framebuffer_response  *fb;
        limine_executable_address_response *addr;
        limine_module_response       *modules;
        limine_memmap_response       *mmaps;
        limine_mp_response           *mp;
    };


    class KSystem;
}


class idk::KSystem
{
private:
    uint64_t m_uptime_ticks;
    uint64_t m_uptime_msec;

    MemoryMap m_mmap_buf[32];
    inplace_vector<MemoryMap>    m_mmaps;
    inplace_vector<limine_file*> m_modules;
    inplace_vector<ExecHeader>   m_execs;
    inplace_vector<kn_TTY*>      m_ttys;


    void _load_mmaps();
    void _load_modules();

public:
    Krequests m_reqs;
    idk::CPU cpu0, cpu1, cpu2, cpu4;
    idk::Video video;

    KSystem( const Krequests& );
    // void init();

    auto        &getMmaps() { return m_mmaps; }
    uint64_t     getHHDM();
    limine_file *getModule( const char *label );
    int          execute( const char *filepath, int argc, char **argv );
    void         lsmem();

};





// namespace idk
// {
//     struct CPU;
//     struct SystemClock;
//     class  System;
//     System &getSystem();


//     inline constexpr
//     void hcf()
//     {
//         while (true)
//         {
//             asm("hlt");
//         } 
//     }

// }


// struct idk::CPU
// {
//     inline static char _fxsave[512] __attribute__((aligned(16)));
//     inline constexpr void fxsave() { asm volatile("fxsave %0 "::"m"(_fxsave)); }
//     inline constexpr void fxrstor() { asm volatile("fxrstor %0" : : "m"(_fxsave)); }
//     void enableSSE();
// };



// struct idk::SystemClock
// {
//     uint64_t uptime_ticks;
//     uint64_t uptime_msec;
//     void update();
// };



// class idk::System
// {
// private:
//     idk::linear_allocator     m_mainblock;
//     inplace_vector<Terminal*> m_terminals;

// public:
//     idk::CPU cpu0, cpu1, cpu2, cpu4;
//     idk::Terminal *tty0, *tty1;
//     // idk::WinFrameBase *winroot;

//     SystemClock         clock;
//     idk::Video          video;
//     // idk::Scheduler      scheduler;

//     // System( const idk_BootInfo &info );
//     System();
//     ~System();
//     void init( const idk_BootInfo &info );
//     void update();

//     Terminal *createTerminal( int w, int h );
// };

