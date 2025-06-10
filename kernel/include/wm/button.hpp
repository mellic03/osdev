#pragma once
#include "element.hpp"


namespace wm
{
    struct guiButton: public guiElement
    {
    private:
        u8vec4 m_color0 = u8vec4(100, 100, 100, 255);
        u8vec4 m_color1 = u8vec4(200, 200, 200, 255);
        vec4   m_colorA = vec4(0.9f, 0.2f, 0.2f, 1.0f);
        vec4   m_colorB = vec4(0.4f, 0.8f, 0.8f, 1.0f);
        bool   m_hovered = false;

    public:
        char m_label[32];
        void (*m_onClick)() = [](){  };

        guiButton( const char *label, const ivec2 &tl, const ivec2 &sp,
                   void (*click)()=nullptr );

        guiButton( const char *label, void (*click)()=nullptr )
        :   guiButton(label, ivec2(0, 0), ivec2(100, 50), click) {  };

        virtual void onHoverEnter( guiMouse& ) override;
        virtual void onHoverExit( guiMouse& ) override;
        virtual void onClick( guiMouse& ) override { m_onClick(); };
        virtual void draw( guiFramebuffer& ) override;

    };

}





