#pragma once

#include <stddef.h>
#include <stdint.h>

#include <kernel/font.hpp>
#include <kinplace/inplace_vector.hpp>
#include <kmemory/memory.hpp>
#include <kernel/terminal.hpp>
#include <kmath/vec.hpp>


struct limine_framebuffer;
struct limine_module_response;

namespace idk
{
    template <typename T>
    struct buf_wrapper;

    class WinFrameBase;
    class WinFrameSolid;
    class WinFrameTTY;

    class Video;
}




// ---------------------------------------------------------------------------------------------
template <typename T>
struct idk::buf_wrapper
{
    int W;
    T *buf;

    buf_wrapper()
    :   W(0), buf(0x00) {  };

    buf_wrapper( int w, T *b )
    :   W(w), buf(b) {  };

    inline constexpr
    T *operator[](int i)
    {
        return &(buf[W*i]);
    };

    
    inline constexpr
    const T *operator[](int i) const
    {
        return &(buf[W*i]);
    };

};
// ---------------------------------------------------------------------------------------------






// ---------------------------------------------------------------------------------------------
class idk::WinFrameBase
{
private:
    WinFrameBase *cbuf[8];

public:
    ivec2 m_tl;
    ivec2 m_sp;
    uvec4 m_bg;
    float m_depth;

    WinFrameBase *m_parent;
    idk::inplace_vector<WinFrameBase*> m_children;

    WinFrameBase( const vec2 &tl, const vec2 &sp );
    virtual ~WinFrameBase() = default;
    virtual void render( idk::Video &video ) = 0;

    void giveChild( WinFrameBase* );

};
// ---------------------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------------------
class idk::WinFrameSolid: public idk::WinFrameBase
{
private:

public:
    using WinFrameBase::WinFrameBase;
    virtual void render( idk::Video &video ) override;

};
// ---------------------------------------------------------------------------------------------





// ---------------------------------------------------------------------------------------------
class idk::WinFrameTTY: public idk::WinFrameBase
{
private:
    idk::Terminal   *m_tty;
    idk::FontBuffer *m_font;

public:
    WinFrameTTY( idk::Terminal *tty, idk::FontBuffer *font );
    virtual void render( idk::Video &video ) override;
};
// ---------------------------------------------------------------------------------------------









// idk::Video
// ---------------------------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------------------------


