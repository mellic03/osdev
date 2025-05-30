#include <wm/element.hpp>
#include <wm/render.hpp>
#include <kassert.h>
#include <string.h>


wm::guiElement::guiElement( const ivec2 &tl, const ivec2 &sp )
:   m_tl(tl), m_sp(sp)
{
    static uint8_t count = 71;

    m_color[0] = (count); count = (count+63) % 255;
    m_color[1] = (count); count = (count+63) % 255;
    m_color[2] = (count); count = (count+63) % 255;
    m_color[3] = 255;
};




void wm::guiElement::addChild( guiElement *E )
{
    E->m_parent = this;

    if (m_child == nullptr)
        m_child = E;
    else
        m_child->addSibling(E);
}


void wm::guiElement::addSibling( guiElement *E )
{
    guiElement *curr = this;

    while (curr->m_next)
    {    
        curr = curr->m_next;
    }

    curr->m_next = E;
    E->m_prev = curr;
    E->m_parent = m_parent;
}


void wm::guiElement::makeFirst()
{
    guiElement *F = m_parent->m_child;
    guiElement *L = m_prev;
    guiElement *R = m_next;

    if (F == this) return;
    if (L) L->m_next = R;
    if (R) R->m_prev = L;

    if (m_parent)
    {
        m_parent->m_child = this;
        F->m_prev = this;
        m_prev = F;
        m_next = F;
    }
}


void wm::guiElement::makeLast()
{
    if (!m_next)
        return;

    guiElement *last = this;
    guiElement *F = m_parent->m_child;
    guiElement *L = m_prev;
    guiElement *R = m_next;

    if (L) L->m_next = R;
    if (R) R->m_prev = L;

    while (last->m_next)
        last = last->m_next;

    last->m_next = this;
    m_prev = last;
    m_next = nullptr;

    if (F == this)
    {
        m_parent->m_child = R;
    }
}


void wm::guiElement::draw( guiFramebuffer &dst )
{
    // u8vec4 color(255);
    // for (int i=0; i<3; i++)
    //     color[i] = CPU::getTSC() % 255;

    wm::guiRenderRect(dst, m_color, m_tl.x, m_tl.y, m_sp.x, m_sp.y);

    auto *curr = m_child;
    while (curr)
    {
        curr->draw(dst);
        curr = curr->m_next;
    }
}







wm::guiButton::guiButton( const char *label, const ivec2 &tl, const ivec2 &sp, void (*fn)() )
:   guiElement(tl, sp),
    m_onClick(fn)
{
    memset(m_label, '\0', sizeof(m_label));
    strncpy(m_label, label, sizeof(m_label)-1);
}




void wm::guiButton::onHoverEnter( guiMouse& )
{
    // float a = 0.01f;
    // m_color2 = (1.0f-a)*m_color0 + a*m_color1;
    // m_color  = u8vec4(255.0f * m_color2);
    m_color = m_color0;
}


void wm::guiButton::onHoverExit( guiMouse& )
{
    // float a = 0.01f;
    // m_color2 = (1.0f-a)*m_color1 + a*m_color0;
    // m_color  = u8vec4(255.0f * m_color2);
    m_color = m_color1;
}


void wm::guiButton::draw( guiFramebuffer &dst )
{
    wm::guiRenderRect(dst, m_color, m_tl.x, m_tl.y, m_sp.x, m_sp.y);

}


