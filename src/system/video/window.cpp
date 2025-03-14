#include "./window.hpp"


idk::VWindow::VWindow( int id, const ivec2 &cnr, const ivec2 &ext, const uvec4 &color )
:   ID(id),
    corner(cnr),  extents(ext), bg(color),
    x(corner.x),  y(corner.y),
    w(extents.x), h(extents.y),
    term(nullptr),
    font(nullptr)
{
    term    = nullptr;
    font    = nullptr;
    corner  = cnr;
    extents = ext;
    bg      = color;

    for (int i=0; i<4; i++)
    {
        children[i] = nullptr;
    }
}