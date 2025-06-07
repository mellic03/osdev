#include <wm/mouse.hpp>
#include <wm/element.hpp>


static wm::guiElement*
find_hovered( wm::guiElement *curr, const ivec2 &xy )
{
    auto *child = curr->m_child;

    if (!child)
        return curr;

    while (child->m_next)
        child = child->m_next;

    while (child)
    {
        if (wm::guiElement::isOver(child, xy))
            return find_hovered(child, xy);
        child = child->m_prev;
    }

    return curr;
}



void wm::guiMouse::update( const knl::MsState &mscurr, guiElement *root )
{
    static knl::MsState msprev;

    this->delta = mscurr.pos() - msprev.pos();
    this->xy = mscurr.pos();

    if (this->delta != ivec2(0, 0))
    {
        updateHovered(root);
    }

    if (mscurr.l && !msprev.l) // left down
    {
        this->focused = this->hovered;

        if (focused && (focused != root))
        {
            focused->makeLast();
            focused->onClick(*this);
            this->dragging = true; 
        }
    }

    if (!mscurr.l && msprev.l) // left up
    {
        this->dragging = false; 
    }

    msprev = mscurr;
}



void wm::guiMouse::updateHovered( wm::guiElement *root )
{
    auto *prev = this->hovered;
    auto *curr = find_hovered(root, this->xy);

    if (curr != prev)
    {
        if (prev) prev->onHoverExit(*this);
        if (curr) curr->onHoverEnter(*this);
    }

    else if (focused)
    {
        focused->onMouseMove(*this);
    }

    this->hovered = curr;
}



