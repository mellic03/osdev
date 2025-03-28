#pragma once

#include "kwin.hpp"
#include <kdef.h>
#include <kinplace/inplace_vector.hpp>


namespace kwin
{
    class Context;

    struct Style;
    class Frame;
    // class BorderFrame;
}


struct kwin::Style
{
    vec4 border;

    Style( const vec4 &bd = vec4(0.0f) )
    :   border(bd)
    {

    }
};


class kwin::Frame
{
private:
    Frame *m_fbuf[16];

public:
    ivec2 m_local, m_world;
    ivec2 &m_tl, m_sp;
    vec4 m_col;
    kwin::Style m_style;

    kwin::Frame *m_parent;
    idk::inplace_vector<kwin::Frame*> m_children;

    Frame( ivec2 tl, ivec2 sp, vec4 c = vec4(1.0f), const Style &style = Style() );
    void updateTransforms();

    template <typename frame_type, typename... Args>
    frame_type *giveChild( Args... args )
    {
        frame_type *F = new frame_type(args...);
        F->m_parent = this;
        m_children.push_back(dynamic_cast<kwin::Frame*>(F));
        return F;
    }


    virtual ~Frame() = default;
    virtual void draw( kwin::Context& );
};




// class kwin::BorderFrame: public kwin::Frame
// {
// public:

//     using kwin::Frame::Frame;
//     virtual void draw( kwin::Context& ) override;

// };



