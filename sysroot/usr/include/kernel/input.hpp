#pragma once
#include <stdint.h>
#include <static_vector.hpp>
#include <atomic>



// struct EventEmitter
// {

// };


namespace kinput
{
    struct MsData
    {
        std::atomic_int x, y;
        int l, m, r;
    };

    extern MsData mousedata;

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


    #ifndef MODULE_SYMBOLS
        void writeMsData( const kinput::MsData *data );
        void readMsData( kinput::MsData *data );
        void triggerInputEvent( uint32_t event );
        void triggerMouseEvent( uint32_t btn, uint32_t event );
        void onInputEvent( uint32_t event );
    #endif
}

