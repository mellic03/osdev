#pragma once
#include <cringe/vec.hpp>
#include <kernel/event.hpp>


namespace wm
{
    struct guiElement;

    struct guiMouse
    {
        bool dragging = false;
        struct {
            ivec2 start_xy   = ivec2(0, 0);
            ivec2 start_lpos = ivec2(0, 0);
        } drag;

        ivec2 delta     = ivec2(0, 0);
        ivec2 xy        = ivec2(0, 0);
        guiElement *hovered = nullptr;
        guiElement *focused = nullptr;

        // void
        void update( const knl::MsState &ms, guiElement *root );
        void updateHovered( guiElement *root );
    };
}

