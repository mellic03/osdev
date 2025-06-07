#include <wm/element.hpp>
#include <wm/render.hpp>
#include <kassert.h>
#include <string.h>


wm::guiElement*
wm::guiElement::findRoot( guiElement *E )
{
    guiElement *curr = E;
    while (E->m_parent)
        E = E->m_parent;
    return curr;
}

bool
wm::guiElement::isOver( guiElement *E, const ivec2 &p )
{
    auto &tl = E->m_gpos;
    auto &sp = E->m_gsp;
    bool x = (tl.x < p.x) && (p.x < tl.x+sp.x);
    bool y = (tl.y < p.y) && (p.y < tl.y+sp.y);
    return x && y;
}



// wm::guiElementBounds
// wm::guiElement::getBounds( guiElement *E, const guiElementBounds &Bp )
// {
//     guiElementBounds B = Bp;

//     B.tl = vec_clamp(B.tl+E->m_lpos, Bp.tl, Bp.bl());
//     B.sp.x = std::min(B.sp.x, E->m_lsp.x);
//     B.sp.y = std::min(B.sp.y, E->m_lsp.y);
//     B.sp.x = std::min(B.sp.x, (B.bl()-B.tl).x);
//     B.sp.y = std::min(B.sp.y, (B.bl()-B.tl).y);

//     return B;
// }



wm::guiElement::guiElement( const ivec2 &tl, const ivec2 &sp )
:   m_lpos(tl), m_gpos(tl), m_lsp(sp), m_gsp(sp)
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

    if (m_root == nullptr)
        m_root = findRoot(this);
    E->m_root = m_root;

    if (m_child == nullptr)
        m_child = E;
    else
        m_child->addSibling(E);
}


void wm::guiElement::addSibling( guiElement *E )
{
    guiElement *curr = this;
    while (curr->m_next)
        curr = curr->m_next;
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





void wm::guiElement::updateBounds( const ivec2 &tl, const ivec2 &sp )
{
    if (m_style.fillBounds)
    {
        m_gpos = tl;
        m_gsp  = sp;
        return;
    }

    m_gpos  = tl + m_lpos + m_margin;
    m_gsp.x = std::min(m_lsp.x, sp.x - 2*m_margin.x);
    m_gsp.y = std::min(m_lsp.y, sp.y - 2*m_margin.y);
}


void wm::guiElement::update( const ivec2 &tl, const ivec2 &sp )
{
    updateBounds(tl, sp);

    for (auto *E: *this)
    {
        E->update(m_gpos+m_padding, m_gsp-2*m_padding);
    }
}


void wm::guiElement::draw( guiFramebuffer &dst )
{
    if (m_style.solidColor)
        wm::guiRenderRect(dst, m_color, m_gpos, m_gsp);
    else
        wm::guiRenderBox(dst, m_color, m_gpos, m_gsp);

    for (auto *E: *this)
    {
        E->draw(dst);
    }

}




