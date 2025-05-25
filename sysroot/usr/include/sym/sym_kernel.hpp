#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>

namespace knl
{
    struct MsState;
    struct MsEvent;
    struct KbEvent;
}

namespace usrknl
{
    inline void (*panic)(const char*);
    inline void (*hcf)();
    inline ModuleInterface *(*findModule)(uint64_t, uint64_t);

    inline bool (*readMsState)(knl::MsState&);
    inline void (*writeMsState)(const knl::MsState&);
    inline bool (*readMsEvent)(knl::MsEvent&);
    inline void (*writeMsEvent)(const knl::MsEvent&);
    inline bool (*readKbEvent)(knl::KbEvent&);
    inline void (*writeKbEvent)(const knl::KbEvent&);

    inline int (*listenMsEvent)( void (*)(const knl::MsEvent&) );
    inline int (*listenKbEvent)( void (*)(const knl::KbEvent&) );
    inline void (*emitMsEvent)( const knl::MsEvent& );
    inline void (*emitKbEvent)( const knl::KbEvent& );
    inline void (*forgetMsEvent)( int );
    inline void (*forgetKbEvent)( int );

}

