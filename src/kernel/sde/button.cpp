#include "button.hpp"
#include "sde.hpp"
#include "../gx/gx.hpp"
#include <kernel/clock.hpp>
#include <kernel/log.hpp>


__attribute__((used))
static void onHoverStart( void *arg )
{
    syslog::kprintf("[onHoverStart]!!\n");
    auto &F = *(sde::Button*)arg;
    F.m_style.fill_color =  F.m_fillColorHovered;

}

__attribute__((used))
static void onHoverStop( void *arg )
{
    syslog::kprintf("[onHoverStop]!!\n");
    auto &F = *(sde::Button*)arg;
    F.state.hovered = false;
    F.m_style.fill_color =  F.m_fillColor;
}


sde::Button::Button( int x, int y, int w, int h )
:   Frame(x, y, w, h)
{
    _onHoverStart = onHoverStart;
    _onHoverStop  = onHoverStop;

    m_fillColor = vec4(0.0, 0.0, 0.0, 1.0);
    m_fillColorHovered = vec4(0.8, 0.8, 0.8, 1.0);

    m_style.fill_color = m_fillColor;
}


void
sde::Button::update()
{
    Frame::update();


}



void
sde::Button::draw()
{
    Frame::draw();

}




// sde::Frame*
// sde::makeButton( int x, int y, int w, int h )
// {
//     sde::Frame *F = (sde::Frame*)kmalloc(sizeof(sde::Frame));

//     F->m_corner = ivec2(x, y);
//     F->m_span   = ivec2(w, h);

//     F->state = {
//         .hovered = false
//     };

//     F->onHoverStart = onHoverStart;
//     F->onHoverStop   = onHoverStop;

//     return F;
// }

