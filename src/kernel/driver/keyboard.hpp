#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kstackframe.h>
#include <kfstream.hpp>
#include "interface.hpp"


namespace kdriver::ps2_kb
{
    enum KeyEvent_: uint32_t
    {
        KeyEvent_UP     = 1<<0,
        KeyEvent_SHIFT  = 1<<1,
        KeyEvent_CTRL   = 1<<2,
        KeyEvent_ALT    = 1<<3,
        KeyEvent_L      = 1<<4,
        KeyEvent_R      = 1<<5,
        KeyEvent_U      = 1<<6,
        KeyEvent_D      = 1<<7,
    };

    struct KeyEvent
    {
        uint8_t mask;
        uint8_t key;
    };

    void irq_handler( kstackframe* );
    void driver_main( void* );

}


void keyboard_irq_handler( kstackframe* );
char scode_getchar( uint8_t );

