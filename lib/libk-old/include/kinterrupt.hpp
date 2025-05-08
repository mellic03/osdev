#pragma once
#include <stdint.h>


template <uint8_t isrno>
void KInterrupt()
{
    asm volatile(
        "int %0"
        :
        : "i" (isrno)
    );
}


