#pragma once
#include <stdint.h>



inline static uint32_t ckStackPointer()
{
    uint32_t *sp;
    asm("mov %%esp, %0" : "=r" (sp));

    return (uint32_t)(sp);
}


