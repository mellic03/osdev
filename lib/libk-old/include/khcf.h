#pragma once

inline void kernel_hcf()
{
    while (1) { asm volatile ("cli; hlt"); }
}


