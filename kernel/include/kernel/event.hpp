#pragma once
#include <stdint.h>
#include <cringe/vec.hpp>


namespace knl
{
    struct MsState
    {
        int x, y;

        bool l,  m,  r;
        // bool lp, mp, rp;

        MsState( const ivec2 &p = ivec2(128, 128) )
        : x(p.x), y(p.y), l(false), m(false), r(false) {  };
        //   lp(false), mp(false), rp(false) {  };
    
        ivec2 pos() { return ivec2(x, y); };
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

    // bool readMsState( MsState& );
    // void writeMsState( const MsState& );

    // bool readMsEvent( MsEvent& );
    // void writeMsEvent( const MsEvent& );

    // bool readKbEvent( KbEvent& );
    // void writeKbEvent( const KbEvent& );

    // int listenMsEvent( void (*callback)(const MsEvent&) );
    // int listenKbEvent( void (*callback)(const KbEvent&) );

    // void emitMsEvent( const MsEvent& );
    // void emitKbEvent( const KbEvent& );

    // void forgetMsEvent( int callback_id );
    // void forgetKbEvent( int callback_id );

    // void onKbEvent( KbEvent event, BtnAction_ action, void (*callback)(void) )
    // {
        
    // }

    // struct MsState
    // {
    //     int x, y;

    //     struct {
    //         bool l, m, r;
    //     } prevDown, currDown;

    //     MsState( const ivec2 &p = ivec2(128, 128) )
    //     : x(p.x), y(p.y)
    //     {
    //         prevDown = {false, false, false};
    //         currDown = {false, false, false};
    //     };
    
    //     ivec2 pos() { return ivec2(x, y); };
    // };

    // struct KbState
    // {
    //     bool prevDown[256], currDown[256];
    // };


}

