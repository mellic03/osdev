#pragma once
#include <stdint.h>
#include <static_vector.hpp>
#include <atomic>
#include <functional>
#include <cringe/vec.hpp>



// struct EventEmitter
// {

// };


namespace knl
{
    struct MsState
    {
        int x, y;

        struct {
            bool l, m, r;
        } prevDown, currDown;

        MsState( const ivec2 &p = ivec2(128, 128) )
        : x(p.x), y(p.y)
        {
            prevDown = {false, false, false};
            currDown = {false, false, false};
        };
    
        ivec2 pos() { return ivec2(x, y); };
    };


    struct KbEvent
    {
        uint8_t key;
    };

    struct KbState
    {
        bool prevDown[256], currDown[256];
    };



}



namespace kinput
{
    struct MsData
    {
        int x, y;

        struct {
            bool l, m, r;
        } prevDown, currDown;

        MsData( const ivec2 &p = ivec2(128, 128) )
        : x(p.x), y(p.y)
        {
            prevDown = {false, false, false};
            currDown = {false, false, false};
        };
    
        ivec2 pos() { return ivec2(x, y); };
    };

    struct MsCallbacks
    {
        struct {
            void (*l)() = nullptr;
            void (*m)() = nullptr;
            void (*r)() = nullptr;
        } onDown[4], onUp[4];
    };

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
        void writeMsData( const kinput::MsData& );
        void readMsData( kinput::MsData& );
        void writeMsCallbacks( const kinput::MsCallbacks& );
        void readMsCallbacks( kinput::MsCallbacks& );

        // void writeKbCallbacks( const kinput::KbCallbacks& );
        // void readKbCallbacks( kinput::KbCallbacks& );
    #endif
}

