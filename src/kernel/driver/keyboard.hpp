#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kstackframe.h>



namespace kdriver::ps2_kb
{
    enum KeyModifier: uint8_t
    {
        MODIFIER_SHIFT  = 1 << 0,
        MODIFIER_CTRL   = 1 << 1,
        MODIFIER_ALT    = 1 << 2
    };

    struct Packet
    {
        uint8_t modifier;
        uint8_t scancode;
    };

    void driver_main( void* );

}


void keyboard_irq_handler( kstackframe* );
char scode_getchar( uint8_t );

