#pragma once
#include <kernel/event.hpp>
#include <kernel/ringbuffer.hpp>
#include <cringe/vec.hpp>
#include <kmemxx.hpp>
#include "mouse.hpp"


namespace wm
{
    struct guiFramebuffer;

    struct guiElement
    {
        ivec2 m_tl, m_sp;
        u8vec4 m_color = u8vec4(200, 200, 200, 255);

        guiElement *m_parent=nullptr, *m_child=nullptr;
        guiElement *m_prev  =nullptr, *m_next =nullptr;

        void addChild( guiElement* );
        void addSibling( guiElement* );
        void makeFirst();
        void makeLast();

        guiElement( const ivec2 &tl, const ivec2 &sp );

        bool isOver( const ivec2 &p )
        { 
            bool x = (m_tl.x < p.x) && (p.x < m_tl.x+m_sp.x);
            bool y = (m_tl.y < p.y) && (p.y < m_tl.y+m_sp.y);
            return x && y;
        }

        virtual void onHoverEnter( guiMouse& ) {  };
        virtual void onHoverExit( guiMouse& ) {  };
        virtual void onMouseMove( guiMouse& ) {  };
        virtual void onClick( guiMouse& ) {  };
        virtual void onKey( const knl::KbEvent& ) {  };
        virtual void draw( guiFramebuffer& );
    };

    
    struct guiButton: public guiElement
    {
    private:
        u8vec4 m_color0 = u8vec4(100, 100, 100, 255);
        u8vec4 m_color1 = u8vec4(200, 200, 200, 255);

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
        virtual void draw( guiFramebuffer& );

    };

}





