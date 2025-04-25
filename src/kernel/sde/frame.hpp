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
    bool  fill         = false;
    int   border       = 0;
    vec4  fill_color   = vec4(1, 1, 1, 0.8);
    vec4  border_color = vec4(0.75, 0.85, 0.75, 1.0);
    int   xalign       = -1;
    int   yalign       = -1;
    ivec4 padding      = ivec4(2);
    ivec4 margin       = ivec4(2);
};


class sde::Frame
{
protected:
    ivec2 m_prev_wcorner;
    bool  m_dirty = true;
    void _update_state();

public:
    // inline static float       zNearest  = 0.0f;
    // inline static sde::Frame *focus     = nullptr;
    // inline static sde::Frame *focusRoot = nullptr;
    // static void        findFocus( sde::Frame* );
    // static sde::Frame *findRoot( sde::Frame* );
    Style m_style;
    Style m_style2;
    ivec2 m_corner;
    ivec2 m_wcorner;
    ivec2 m_span;
    float m_depth  = 0.99f;

    struct {
        bool hovered = false;
    } state;

    using fn_type = void (*)(void*);
    fn_type _onHoverStart   = nullptr;
    fn_type _onHoverStop     = nullptr;
    fn_type _onLeftPress    = nullptr;
    fn_type _onLeftRelease  = nullptr;
    fn_type _onRightPress   = nullptr;
    fn_type _onRightRelease = nullptr;


    // int &m_gx, &m_gy;
    // int &m_lx, &m_ly;
    // int &m_w,  &m_h;

    sde::Frame *m_parent;
    std::vector<sde::Frame*> m_children;

    Frame( int x, int y, int w, int h );
    virtual ~Frame();
    
    bool is_dirty();
    void make_dirty();
    void make_clean();
    
    virtual void update();
    virtual void draw();

    // template <typename frame_type, typename... Args>
    // frame_type *giveChild( Args... args );

    // template <typename frame_type>
    // frame_type *giveChild( ivec2 offset, frame_type *F );

    // void removeChild( void* );

};



// template <typename frame_type, typename... Args>
// frame_type*
// sde::Frame::giveChild( Args... args )
// {
//     frame_type *F = new frame_type(args...);
//     F->m_parent = this;
//     m_children.push_back(dynamic_cast<sde::Frame*>(F));
//     return F;
// }


// template <typename frame_type>
// frame_type*
// sde::Frame::giveChild( ivec2 offset, frame_type *F )
// {
//     F->m_local = offset;
//     F->m_local.x += m_style.padding[0];

//     if (offset.x < m_span.x/2)
//         F->m_span.x = m_span.x - 2*offset.x;

//     if (offset.y < m_span.y/2)
//         F->m_span.y = m_span.y - 2*offset.y;

//     F->m_parent = this;
//     m_children.push_back(dynamic_cast<sde::Frame*>(F));
//     return F;
// }

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




