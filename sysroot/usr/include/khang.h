#pragma once


namespace kernel
{
    inline void halt()
    {
        while (true) { asm volatile ("hlt"); }
    }

    inline void hang()
    {
        while (true) { asm volatile ("cli; hlt"); }
    }
}




