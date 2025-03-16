#pragma once

#include <stddef.h>
#include <stdint.h>
#include "../../bootinfo.hpp"
#include "buf_wrapper.hpp"
#include "color.hpp"
#include "font.hpp"
#include "window.hpp"
#include "memory/linear_allocator.hpp"
#include "data_structures/inplace_vector.hpp"

#include <algorithm.hpp>
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

    class Video;
}



class idk::Video
{
private:
    int m_W;
    int m_H;
    buf_wrapper<uint32_t> m_frontbuffer;
    buf_wrapper<uint32_t> m_backbuffer;
    buf_wrapper<float>    m_depthbuffer;


    inplace_vector<WinFrameBase*> m_winframes;
    inplace_vector<FontBuffer>    m_fonts;


public:
    void init( idk::linear_allocator&, limine_framebuffer* );
    void update();
    void swapBuffers();

    void rect( ivec2 cnr, ivec2 span, const uvec4 &color, float z=0.9999f );
    void blit( ivec2 dsttl, ivec2 srctl, ivec2 sp, const buf_wrapper<uint32_t>&, float z=0.9999f );

    void loadFonts( limine_module_response* );
    auto &getFonts() { return m_fonts; }


    template <typename wframe_type, typename... Args>
    wframe_type *createWinFrame( Args... args );

    template <typename wframe_type, typename... Args>
    wframe_type *createWChildFrame( WinFrameBase *parent, Args... args );

};





template <typename wframe_type, typename... Args>
wframe_type*
idk::Video::createWinFrame( Args... args )
{
    wframe_type *W = new wframe_type(args...);
    m_winframes.push_back(dynamic_cast<WinFrameBase*>(W));
    return W;
}



template <typename wframe_type, typename... Args>
wframe_type*
idk::Video::createWChildFrame( WinFrameBase *parent, Args... args )
{
    wframe_type *W = new wframe_type(args...);
    parent->giveChild(W);
    return W;
}


