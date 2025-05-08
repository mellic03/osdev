#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kernel/kscancode.h>
#include "interface.hpp"


struct hwPS2Keyboard: public hwDriverInterface
{
    hwPS2Keyboard();
    virtual void loadIrqHandler() final;
};


namespace kdriver::ps2_kb
{
    void irq_handler( intframe_t* );
    void driver_main( void* );
}

void keyboard_irq_handler( intframe_t* );

