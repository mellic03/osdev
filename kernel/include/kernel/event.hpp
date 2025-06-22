#pragma once
#include <stdint.h>
#include <cringe/vec.hpp>


namespace knl
{
    struct MsState
    {
        int x, y;

        bool l,  m,  r;

        MsState( const ivec2 &p = ivec2(128, 128) )
        : x(p.x), y(p.y), l(false), m(false), r(false) {  };
    
        ivec2 pos() const { return ivec2(x, y); };
    };

    enum MsEvent_: uint8_t
    {
        MsEvent_Invalid = 0,
        MsEvent_LDn,
        MsEvent_LUp,
        MsEvent_RDn,
        MsEvent_RUp,
        MsEvent_X,
        MsEvent_Y,
    };

    struct MsEvent
    {
        MsEvent_ type;
        int data;
    };

    struct KbEvent
    {
        uint8_t mask;
        uint8_t key;
    };

}

