#pragma once

#include <stddef.h>
#include <stdint.h>

#include "buf_wrapper.hpp"
#include "font.hpp"
#include "winframe.hpp"
#include <kinplace/inplace_vector.hpp>
#include <kernel/memory.hpp>
#include <kmath/vec.hpp>


extern "C"
{
    struct limine_framebuffer;
    struct limine_framebuffer_response;
    struct limine_module_response;
}

namespace idk
{
    class Video;
}







// idk::Video
// ---------------------------------------------------------------------------------------------
class idk::Video
{
private:
    int m_W;
    int m_H;
    buf_wrapper<uint32_t> m_frontbuffer;
    buf_wrapper<uint32_t> m_backbuffer;
    // buf_wrapper<float>    m_depthbuffer;

    inplace_vector<WinFrameBase*> m_winframes;
    inplace_vector<FontBuffer>    m_fonts;


public:
    void init( limine_framebuffer_response* );
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
// ---------------------------------------------------------------------------------------------


