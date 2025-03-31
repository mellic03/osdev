#pragma once

#include <kdef.h>
#include <kmath/vec.hpp>
#include <vector>


namespace sde
{
    class Context;
    struct Style;
    class Frame;
    // class BorderFrame;
    // class SolidFrame;
}


struct sde::Style
{
    bool fill         = false;
    bool border       = true;
    vec4 fill_color   = vec4(1, 1, 1, 0.8);
    vec4 border_color = vec4(0.75, 0.85, 0.75, 1.0);
};


class sde::Frame
{
public:
    ivec2 m_global, m_local;
    ivec2 m_span;
    
    int &m_gx, &m_gy;
    int &m_lx, &m_ly;
    int &m_w,  &m_h;
    float z = 0.99f;

    sde::Style m_style;
    sde::Frame *m_parent;
    std::vector<sde::Frame*> m_children;

    Frame( ivec2 tl=ivec2(0), ivec2 sp=ivec2(128) );
    virtual ~Frame();

    template <typename frame_type, typename... Args>
    frame_type *giveChild( Args... args )
    {
        frame_type *F = new frame_type(args...);
        F->m_parent = this;
        m_children.push_back(dynamic_cast<sde::Frame*>(F));
        return F;
    }

    template <typename frame_type>
    frame_type *giveChild( ivec2 offset, frame_type *F )
    {
        F->m_local = offset;
    
        if (offset.x < m_span.x/2)
            F->m_span.x = m_span.x - 2*offset.x;
  
        if (offset.y < m_span.y/2)
            F->m_span.y = m_span.y - 2*offset.y;

        F->m_parent = this;
        m_children.push_back(dynamic_cast<sde::Frame*>(F));
        return F;
    }


    virtual void draw();
};


// class sde::BorderFrame: public sde::Frame
// {
//     BorderFrame( ivec2 tl, ivec2 sp )
//     :   Frame(tl, sp)
//     {
        
//     }

// };


// class sde::SolidFrame: public sde::Frame
// {
//     SolidFrame( ivec2 tl, ivec2 sp )
//     :   Frame(tl, sp)
//     {
        
//     }

// };




