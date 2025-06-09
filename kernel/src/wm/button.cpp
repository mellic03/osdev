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
    m_colorA = vec4(0.9f, 0.2f, 0.2f, 1.0f);
    m_colorB = vec4(0.4f, 0.8f, 0.8f, 1.0f);

    vec4 color = vec4(m_color) / 255.0f;
    color = vec_test(color, m_colorA, 0.5f);
    m_color = u8vec4(255.0f * color);

    // color.r = 0.95f*color.r + 0.05f*m_colorA.r;
    // color.g = 0.95f*color.g + 0.05f*m_colorA.g;
    // color.b = 0.95f*color.b + 0.05f*m_colorA.b;
    // color.a = 0.95f*color.a + 0.05f*m_colorA.a;

    // m_color = m_color0;
}


void wm::guiButton::onHoverExit( guiMouse& )
{
    m_colorA = vec4(0.9f, 0.2f, 0.2f, 1.0f);
    m_colorB = vec4(0.4f, 0.8f, 0.8f, 1.0f);

    vec4 color = vec4(m_color) / 255.0f;
    color = vec_test(color, m_colorB, 0.5f);
    m_color = u8vec4(255.0f * color);

}


void wm::guiButton::draw( guiFramebuffer &dst )
{
    wm::guiRenderRect(dst, m_color, m_gpos, m_gsp);
}