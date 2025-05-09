#pragma once


namespace kernel
{
    inline void hang()
    {
        while (true) { asm volatile ("cli; hlt"); }
    }
}




