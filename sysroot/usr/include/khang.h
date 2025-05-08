#pragma once

inline void khang()
{
    while (1) { asm volatile ("cli; hlt"); }
}


