#pragma once

#include "../kvideo/kvideo.hpp"
#include "frame.hpp"
#include <kdef.h>
#include <kmemxx.h>
#include <kmath/vec.hpp>
#include <idk_vector.hpp>



namespace kwin
{
    class Context;
    class Frame;
}



class kwin::Context
{
private:

public:
    ivec2 m_tl, m_sp;
    kframebuffer<uint32_t> fb;
    kframebuffer<uint32_t> depth;
    idk::vector<kwin::Frame*> frames;

         Context( int w, int h );
    void flush();
    void rectOutline( ivec2 tl, ivec2 sp, uvec4 color );
    void rect( ivec2 tl, ivec2 sp, uvec4 color );

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


