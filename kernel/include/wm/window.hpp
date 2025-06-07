#pragma once
// #include "element.hpp"
#include "frame.hpp"

namespace wm
{

    struct guiWindowBar: guiFrameVSplit
    {
        guiWindowBar();
        // :   guiElement(ivec2(0), ivec2(1)) {  };

        virtual void onHoverEnter( guiMouse& ) override;
        virtual void onHoverExit( guiMouse& ) override;
        virtual void onMouseMove( guiMouse& ) override;
        virtual void onClick( guiMouse& ) override;
    };

    struct guiWindow: guiFrameHSplit
    {
        guiWindow( const ivec2 &tl, const ivec2 &sp );
        virtual void addChild( guiElement* ) override;
    };

    struct guiRoot: guiFrame
    {
        // using guiWindow::guiWindow;
        guiRoot()
        :   guiFrame(ivec2(0, 0), ivec2(1, 1)) {  };

        virtual void update( const ivec2&, const ivec2& ) final;
        virtual void draw( guiFramebuffer& ) final;
    };
}

