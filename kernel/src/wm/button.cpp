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
    m_hovered = true;
}


void wm::guiButton::onHoverExit( guiMouse& )
{
    m_hovered = false;
}


void wm::guiButton::draw( guiFramebuffer &dst )
{
    // m_colorA = vec4(0.9f, 0.5f, 0.5f, 1.0f);
    // m_colorB = vec4(0.5f, 0.5f, 0.9f, 1.0f);
    // float a = (m_hovered) ? 0.02f : 0.02f;

    // vec4 &colorC = (m_hovered) ? m_colorA : m_colorB;

    // if (m_hovered)
    // {
    //     vec4 color = vec4(m_color);
    //     color.r /= 255.0f;
    //     color.r *= 0.5f;
    //     color.r *= 255.0f;
        
    //     m_color = u8vec4(color);
    // }
    m_color = (m_hovered) ? m_color0 : m_color1;

    wm::guiRenderRect(dst, m_color, m_gpos, m_gsp);
}