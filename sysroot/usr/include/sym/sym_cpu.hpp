#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>
#include <kernel/interrupt.hpp>


namespace CPU
{
    void (*installISR)(uint8_t, isrHandlerFn);
    void (*installIRQ)(uint8_t, irqHandlerFn);
}





