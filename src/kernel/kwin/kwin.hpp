#pragma once

#include "../kvideo/kvideo.hpp"
#include "frame.hpp"
#include <kdef.h>
#include <kmemxx.h>
#include <kmath/vec.hpp>
#include <kinplace/inplace_vector.hpp>



namespace kwin
{
    class Context;
    class Frame;
    Context *createContext( int w, int h );
    void     destroyContext( Context* );
}

void kwin_main( void* );



class kwin::Context
{
private:
    Frame *m_fbuf[16];

public:
    vec2 m_tl, m_sp;
    kframebuffer<vec4> m_fb;
    kframebuffer<uint32_t> m_depth;
    idk::inplace_vector<kwin::Frame*> frames;

         Context( int w, int h );
        ~Context();
    void flush();
    void rectOutline( vec2 tl, vec2 sp, vec4 color );
    void rect( vec2 tl, vec2 sp, vec4 color );
    void blit( vec2 tl0, vec2 tl1, vec2 sp, const kframebuffer<vec4>& );

    template <typename frame_type, typename... Args>
    frame_type *createFrame( Args... );
};



template <typename frame_type, typename... Args>
frame_type*
kwin::Context::createFrame( Args... args )
{
    frame_type *F = new frame_type(args...);
    frames.push_back(dynamic_cast<kwin::Frame*>(F));
    return F;
}


