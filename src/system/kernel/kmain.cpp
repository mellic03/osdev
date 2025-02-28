#include <kcommon/memory.hpp>
#include <kcommon/terminal.hpp>
#include <kcommon/module.hpp>
#include <kcommon/multiboot2.hpp>
#include <stdc++/cstdio.hpp>


extern "C" { void kmain( uint32_t, uint32_t ); }

static int ree;


void kmain( uint32_t magic, uint32_t addr ) 
{
    // ckTerminal term(80, 25, (uint16_t*)(0xB8000));
    // ckTerminalInit(term);

    if (magic == addr)
    {
        ree = 0;
    }

    else
    {
        ree = 1;
    }

}