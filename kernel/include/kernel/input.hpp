#pragma once
#include <stdint.h>
#include <static_vector.hpp>
#include <functional>



namespace kernel
{
    struct InputMouseDevice
    {
        int x, y;
        int l, m, r;
    };

    void triggerInputEvent( uint32_t event );
    void onInputEvent( uint32_t event );
}



enum MsBtn_: uint32_t
{
    MsBtn_None = 0,
    MsBtn_Left,
    MsBtn_Mid,
    MsBtn_Right,
};

enum MsEvt_: uint32_t
{
    MsEvt_None = 0,
    MsEvt_Pressed  = 0b0001,
    MsEvt_Released = 0b0010,
    MsEvt_Clicked  = 0b0100,
};

void triggerMouseEvent( uint32_t btn, uint32_t event );


inline void testMouseEvent()
{
    triggerMouseEvent(MsBtn_Left, MsEvt_Clicked);
}

