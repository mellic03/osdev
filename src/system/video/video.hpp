#pragma once

#include <stddef.h>
#include <stdint.h>
#include "../../bootinfo.hpp"
#include "./color.hpp"
#include "./buffer.hpp"
#include "./window.hpp"
#include "../memory/linear_allocator.hpp"
// #include <limine.h>




// // void fb_init( limine_framebuffer& );
// void fb_blit( ck_Rect &src, ck_Rect &dst );
// void fb_rect( const ck_Rect&, const ck_Color& );


namespace idk
{
    struct Rect
    {
        int x, y, w, h;
    };
    
    inline static uint32_t packRGBA( uint32_t r, uint32_t g, uint32_t b, uint32_t a )
    {
        return (b << 24) + (g << 16) + (r << 8) + (a);
    }
}



namespace idk::video
{
    int  init( idk::linear_allocator&, limine_framebuffer* );
    void swapBuffers();

    void renderRect( int x, int y, int w, int h, const uvec4 &color );
    void renderTerminal( idk::VWindow*, idk::Terminal*, FontBuffer* );
    void renderWindow( idk::VWindow*, idk::VWindow *parent );

    void blit( int x0, int y0, FontBuffer &src );

    struct FONTS
    {
        size_t count = 0;
        size_t cap   = 32;
        FontBuffer *fonts;
    };

    int loadFonts( limine_module_response* );
    FONTS &getFonts();
}

