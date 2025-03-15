#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../bootinfo.hpp"
#include "data_structures/inplace_vector.hpp"
#include "memory/memory.hpp"
#include "syscall/syscall.hpp"
#include "terminal/terminal.hpp"
#include "video/video.hpp"
#include "log.hpp"


namespace idk
{
    struct CPU;
    class System;
    idk::System &getSystem();
}


struct idk::CPU
{
    inline static char _fxsave[512] __attribute__((aligned(16)));
    inline constexpr void fxsave() { asm volatile("fxsave %0 "::"m"(_fxsave)); }
    inline constexpr void fxrstor() { asm volatile("fxrstor %0" : : "m"(_fxsave)); }
    void enableSSE();
};



class idk::System
{
private:
    idk::linear_allocator     m_mainblock;
    inplace_vector<Terminal*> m_terminals;

public:
    idk::CPU cpu0, cpu1, cpu2, cpu4;
    idk::Video          video;

    // System();
    int init( const idk_BootInfo &info );

    Terminal *createTerminal( int w, int h );
};

