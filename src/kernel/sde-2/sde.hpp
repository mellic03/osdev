#pragma once

#include "../kvideo/kvideo.hpp"
#include "frame.hpp"
#include "font.hpp"
#include "texture.hpp"

#include <kdef.h>
#include <kmemxx.hpp>
#include <kmath/vec.hpp>



void sde_main( void* );


namespace sde
{
    static constexpr uint16_t PORT_CMD = 0x5DEA;
    static constexpr uint16_t PORT_MSG = 0x5DEB;
    static constexpr uint8_t  SOTEXT   = 2;
    static constexpr uint8_t  EOTEXT   = 3;

    struct MouseState
    {
        ivec2 pos;
        bool ldown, rdown;
        bool lup,   rup;
    };

    extern MouseState  mouse;
    extern ivec2      &mpos;
    extern int        &mx;
    extern int        &my;
    extern sde::Frame *root;
    extern sde::Frame *focus;
    extern sde::Font  *sysfont;
    extern vec4        sysfont_tint;

    void sysfont_putchar( char ch, int x, int y, float z );

    // uint32_t loadBMP( void *header );
    // uint32_t loadTexture( uint32_t gx_format, const char *filepath );

    sde::Frame *createWindow( ivec2 corner, ivec2 span );
    void destroyWindow( Frame* );
}


