#include "frame.hpp"
#include "sde.hpp"
#include <kthread.hpp>
#include <kmalloc.h>
#include <stdio.h>
#include <kernel/memory.hpp>
#include <algorithm>
#include <kernel/log.hpp>

// {
//     using namespace sde;
//     static ivec2 mPrev  = ivec2(0, 0);
//     static ivec2 mDelta = ivec2(0, 0);
//     mDelta = mousexy - mPrev;
//     mPrev  = mousexy;

//     if (mouseleft)
//     {
//         static Frame *prevfocus = nullptr;
//         Frame::focus = findMouseFocus(sde_root);
//         syslog::kprintf("Focus: 0x%lx\n", Frame::focus);

//         if (Frame::focus != prevfocus)
//             syslog::kprintf("Focus change!\n");
//         prevfocus = Frame::focus;
//     }

//     if (Frame::focus)
//     {
//         Frame::focusRoot = Frame::findRoot(Frame::focus);
//         Frame::focus->m_global += mDelta;
//     }
// }




// sde::Frame*
// sde::Frame::findRoot( sde::Frame *frame )
// {
//     Frame *F = frame;

//     while (F->m_parent && F->m_parent != sde::root)
//     {
//         F = F->m_parent;
//     }

//     Frame::focusRoot = F;
//     return F;
// }




sde::Frame::Frame( int x, int y, int w, int h )
:   m_corner  (x, y),
    m_wcorner (x, y),
    m_span    (w, h),
    m_depth   (0.99f),
    m_parent  (nullptr)
{

}


sde::Frame::~Frame()
{
    for (Frame *child: m_children)
    {
        delete child;
    }
}



bool
sde::Frame::is_dirty()
{
    return m_dirty;
}


void
sde::Frame::make_dirty()
{
    sde::Frame *F = this;

    while (F)
    {
        F->m_dirty = true;
        F = F->m_parent;
    }
}


void
sde::Frame::make_clean()
{
    m_dirty = false;

    for (Frame *F: m_children)
    {
        F->make_clean();
    }
}



static bool rect_point_overlap( vec2 tl, vec2 sp, vec2 p )
{
    bool x = (tl.x <= p.x) && (p.x <= tl.x + sp.x);
    bool y = (tl.y <= p.y) && (p.y <= tl.y + sp.y);
    return x && y; 
}



void
sde::Frame::_update_state()
{
    bool  overlaps = rect_point_overlap(m_wcorner, m_span, sde::mpos);
    float zdepth   = gxDepthSample(sde::mx, sde::my);

    if (zdepth < m_depth)
    {
        overlaps = false;
    }

    bool prev_hover = state.hovered;
    bool curr_hover = overlaps;
    state.hovered = curr_hover;

    bool hover_start = !prev_hover && curr_hover;
    bool hover_stop  = prev_hover && !curr_hover;

    if      (hover_start && _onHoverStart) { _onHoverStart(this); }
    else if (hover_stop  && _onHoverStop)  { _onHoverStop(this);  }

    // if (curr_hover)
    // {
    //     if (sde::sysmouse.left  && _onLeftPress)  { _onLeftPress(this);  }
    //     if (sde::sysmouse.right && _onRightPress) { _onRightPress(this); }
    // }

}



void
sde::Frame::update()
{
    _update_state();

    for (Frame *F: m_children)
    {
        F->m_parent = this;
        F->m_depth = m_depth - 0.01f;
        F->update();
    }
}



void
sde::Frame::draw()
{
    m_wcorner = m_corner;
    if (m_parent) m_wcorner += m_parent->m_wcorner;

    gxDrawRect(
        {m_wcorner.x, m_wcorner.y, m_span.x, m_span.y, m_depth},
        m_style.fill, m_style.border,
        m_style.fill_color,
        m_style.border_color
    );

    for (Frame *F: m_children)
    {
        F->m_parent = this;
        F->m_depth = m_depth - 0.01f;
        F->draw();
    }
}



// void
// sde::Frame::removeChild( void *ptr )
// {
//     int idx = -1;

//     for (size_t i=0; i<m_children.size(); i++)
//     {
//         auto *F = m_children[i];

//         if ((uintptr_t)F == (uintptr_t)ptr)
//         {
//             idx = int(i);
//             break;
//         }
//     }

//     if (idx != -1)
//     {
//         m_children[idx] = m_children.back();
//         m_children.pop_back();
//     }
// }



// void
// sde::BorderFrame::draw( sde::Context &ctx )
// {
//     Frame::draw(ctx);
//     ctx.rectOutline(global, span, m_col);
//     // ctx.rect(m_tl+ivec2(24), span-ivec2(48), m_col);
// }


