#pragma once

#include <kdef.h>
#include <kstackframe.h>
#include <kmath/vec.hpp>
#include "interface.hpp"


namespace kdriver::ps2_mouse
{
    hwDriverInterface getInterface();

    enum MouseEvent_: uint32_t
    {
        MouseEvent_DOWN   = 1<<0,
        MouseEvent_UP     = 1<<1,
        MouseEvent_LEFT   = 1<<2,
        MouseEvent_MIDDLE = 1<<3,
        MouseEvent_RIGHT  = 1<<4,
    };

    struct MouseEvent
    {
        float x;
        float y;
    };

    void irq_handler( kstackframe* );
    void driver_main( void* );



    // extern vec2 position;
    // extern vec2 delta;
}


extern uint8_t MousePointer[];

void InitPS2Mouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();


extern bool mouseleft;
extern bool mouseright;
extern ivec2 mousexy;
extern ivec2 mousedelta;

void mouse_irq( kstackframe* );
void mouse_init();
