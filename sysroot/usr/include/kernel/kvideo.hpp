#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cringe/vec.hpp>


namespace cringe
{
    class Font;
}


namespace kvideo
{
    extern ivec2  CSR;
    extern int    W;
    extern int    H;
    extern int    pitch;
    extern size_t BPP;
    extern size_t stride;
    extern size_t nbytes;

    extern uint8_t *frontbuffer;
    extern uint8_t *backbuffer;

    // void setCursor( const ivec2& );
    // const ivec2 &getCursor();
    void initFrontbuffer( uintptr_t fb_res );
    void initBackbuffer( uintptr_t fb_res );

    void setFont( const cringe::Font& );
    void renderGlyph( char ch, int x, int y );
    void renderString( const char *str, const ivec2& );
    // void cursorGlyph( char ch );
    void cursorString( const char *ch );

    void blit( const ivec2 &dsttl, uint8_t *img, int imgw, int imgh,
               const ivec2 &srctl, const ivec2 &srcsp );

    void clearBuffer( uint8_t *buffer );
    void fillColor( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
    void fillBuffer( uint8_t *buffer );
    
    void rect( int x, int y, int w, int h );

    void swapBuffers();
}



