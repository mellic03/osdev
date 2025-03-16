#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "data_structures/inplace_vector.hpp"
#include "memory/memory.hpp"
// #include "process/process.hpp"
#include "syscall/syscall.hpp"
#include "terminal/terminal.hpp"
#include "video/video.hpp"
#include "log.hpp"


namespace idk
{
    struct CPU;
    struct SystemClock;
    class  System;
    System &getSystem();


    inline constexpr
    void hcf()
    {
        while (true)
        {
            asm("hlt");
        } 
    }

}


struct idk::CPU
{
    inline static char _fxsave[512] __attribute__((aligned(16)));
    inline constexpr void fxsave() { asm volatile("fxsave %0 "::"m"(_fxsave)); }
    inline constexpr void fxrstor() { asm volatile("fxrstor %0" : : "m"(_fxsave)); }
    void enableSSE();
};



struct idk::SystemClock
{
    uint64_t uptime_ticks;
    uint64_t uptime_msec;
    void update();
};



class idk::System
{
private:
    idk::linear_allocator     m_mainblock;
    inplace_vector<Terminal*> m_terminals;

public:
    idk::CPU cpu0, cpu1, cpu2, cpu4;
    idk::Terminal *tty0, *tty1;
    // idk::WinFrameBase *winroot;

    SystemClock         clock;
    idk::Video          video;
    // idk::Scheduler      scheduler;

    // System( const idk_BootInfo &info );
    System();
    ~System();
    void init( const idk_BootInfo &info );
    void update();

    Terminal *createTerminal( int w, int h );
};

