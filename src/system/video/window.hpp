#pragma once
#include "./buffer.hpp"
#include "./color.hpp"
#include "./vec.hpp"
#include "system/terminal/terminal.hpp"
#include "system/data_structures/inplace_array.hpp"


namespace idk
{
    struct VWindow;
}


struct idk::VWindow
{
    const int ID;

    ivec2 corner;
    ivec2 extents;
    uvec4 bg;

    int &x, &y;
    int &w, &h;

    idk::Terminal   *term;
    idk::FontBuffer *font;

    VWindow *children[4] = {nullptr, nullptr, nullptr, nullptr};

    VWindow( int id, const ivec2 &cnr, const ivec2 &ext, const uvec4 &color );
};


