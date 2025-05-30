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
        if (child->isOver(xy))
            return find_hovered(child, xy);
        child = child->m_prev;
    }

    return curr;
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



