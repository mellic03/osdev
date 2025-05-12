#pragma once

#include <stddef.h>
#include <stdint.h>

namespace kvideo
{
    extern int    W;
    extern int    H;
    extern int    pitch;
    extern size_t BPP;
    extern size_t stride;
    extern size_t nbytes;

    extern uint8_t *frontbuffer;
    extern uint8_t *backbuffer;

    void clearBuffer( uint8_t *buffer );

    void fillColor( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
    void fillBuffer( uint8_t *buffer );
    
    void rect( int x, int y, int w, int h );

    void swapBuffers();
}



