#pragma once

#include <kdef.h>
#include <math/vec.hpp>
#include "interface.hpp"


struct hwPS2Mouse: public hwDriverInterface
{
    static ivec2 position;
    static bool  left, prev_left;
    static bool  right, prev_right;

    using fn_type = void (*)();
    inline static fn_type onLeftDown  = nullptr;
    inline static fn_type onLeftUp    = nullptr;
    inline static fn_type onRightDown = nullptr;
    inline static fn_type onRightUp   = nullptr;

    hwPS2Mouse();
    virtual void loadIrqHandler() final;

    /** must be [10, 20, 40, 60, 80, 100, 200] samples per second */
    void setSampleRate( uint8_t value );

    /** must be [0, 1, 2, 3], maps to [1, 2, 4, 8] ticks per mm */
    void setResolution( uint8_t value );
    void setScalingNonLinear();
    void setScalingLinear();
};


// namespace kdriver::ps2_mouse
// {
//     void irq_handler( kstackframe* );
//     void driver_main( void* );
// }



// extern uint8_t MousePointer[];

// void InitPS2Mouse();
// void HandlePS2Mouse(uint8_t data);
// void ProcessMousePacket();


// extern bool mouseleft;
// extern bool mouseright;
// extern ivec2 mousexy;
// extern ivec2 mousedelta;

// void mouse_irq( kstackframe* );
// void mouse_init();
