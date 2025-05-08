#pragma once

#include <stddef.h>
#include <stdint.h>

namespace kvideo
{
    // extern int    W;
    // extern int    H;
    // extern int    pitch;
    // extern size_t BPP;
    // extern size_t stride;
    // extern size_t nbytes;

    // extern uint8_t* volatile frontbuffer;
    // extern uint8_t* volatile backbuffer;

    // void initFrontbuffer( uintptr_t fb_res );
    // void initBackbuffer( uintptr_t fb_res );

    void swapBuffers();
    void fill( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
}



