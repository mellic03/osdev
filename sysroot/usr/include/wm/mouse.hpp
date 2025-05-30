#pragma once
#include <cringe/vec.hpp>


namespace wm
{
    struct guiElement;

    struct guiMouse
    {
        bool  dragging  = false;
        // ivec2 dragstart = ivec2(0, 0);
        // ivec2 dragcurr  = ivec2(0, 0);
        ivec2 delta     = ivec2(0, 0);
        ivec2 xy        = ivec2(0, 0);
        guiElement *hovered = nullptr;
        guiElement *focused = nullptr;
    
        void updateHovered( guiElement *root );
    };
}

