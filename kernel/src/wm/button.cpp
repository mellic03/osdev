#include <wm/button.hpp>
#include <wm/render.hpp>
#include <kassert.h>
#include <string.h>

#include <kernel/log.hpp>


wm::guiButton::guiButton( const char *label, const ivec2 &tl, const ivec2 &sp, void (*fn)() )
:   guiElement(tl, sp),
    m_onClick(fn)
{
    memset(m_label, '\0', sizeof(m_label));
    strncpy(m_label, label, sizeof(m_label)-1);
}



void wm::guiButton::onHoverEnter( guiMouse& )
{
    // vec4 color = vec4(m_color) / 255.0f;
    // color = vec_mix(color, m_colorA, 0.05f);
    // m_color = u8vec4(255.0f * color);
    m_color = m_color0;
}


void wm::guiButton::onHoverExit( guiMouse& )
{
    // vec4 color = vec4(m_color) / 255.0f;
    // color = vec_mix(color, m_colorB, 0.05f);
    // m_color = u8vec4(255.0f * color);
    m_color = m_color1;
}


void wm::guiButton::draw( guiFramebuffer &dst )
{
    wm::guiRenderRect(dst, m_color, m_gpos, m_gsp);
}