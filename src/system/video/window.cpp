#include "./window.hpp"


idk::VWindow::VWindow()
:   x   (corner.x),
    y   (corner.y),
    w   (extents.x),
    h   (extents.y),
    children(32, nullptr)
{
    term = nullptr;
    font = nullptr;
}


idk::VWindow::VWindow( const ivec2 &cnr, const ivec2 &ext, const uvec4 &color )
:   VWindow()
{
    term    = nullptr;
    font    = nullptr;
    corner  = cnr;
    extents = ext;
    bg      = color;

    for (int i=0; i<32; i++)
    {
        children[i] = nullptr;
    }
}