#pragma once
#include <kernel/event.hpp>
#include <kernel/ringbuffer.hpp>
#include <cringe/vec.hpp>
#include <kmemxx.hpp>
#include "mouse.hpp"


namespace wm
{
    struct guiFramebuffer;

    struct guiElementBounds
    {
        ivec2 tl, sp;

        guiElementBounds( const ivec2 &cnr=ivec2(0), const ivec2 &ext=ivec2(64) )
        :   tl(cnr), sp(ext) {  };

        guiElementBounds( int x, int y, int w, int h )
        :   tl(x, y), sp(w, h) {  };

        ivec2 bl() const { return tl + sp;     }
        int xmin() const { return tl.x;        }
        int xmax() const { return tl.x + sp.x; }
        int ymin() const { return tl.y;        }
        int ymax() const { return tl.y + sp.y; }
    };


    struct guiElement
    {
        ivec2 m_lpos, m_gpos;
        ivec2 m_lsp,  m_gsp;
    
        struct {
            bool solidColor = false;
            bool fillBounds = false;
            bool fillX = false;
            bool fillY = false;
        } m_style;

        ivec2 m_margin  = ivec2(4);
        ivec2 m_padding = ivec2(4);
        u8vec4 m_color = u8vec4(200, 200, 200, 255);

        guiElement *m_root   = nullptr;
        guiElement *m_parent = nullptr, *m_child = nullptr;
        guiElement *m_prev   = nullptr, *m_next  = nullptr;
        
        guiElement( const ivec2 &tl, const ivec2 &sp );

        static guiElement *findRoot( guiElement* );
        static bool isOver( guiElement*, const ivec2& );
        void updateBounds( const ivec2&, const ivec2& );

        virtual void addChild( guiElement* );
        virtual void addSibling( guiElement* );
        virtual void makeFirst();
        virtual void makeLast();

        virtual void onHoverEnter( guiMouse& ) {  };
        virtual void onHoverExit( guiMouse& ) {  };
        virtual void onMouseMove( guiMouse& ) {  };
        virtual void onClick( guiMouse& ) {  };
        virtual void onKey( const knl::KbEvent& ) {  };

        virtual void update( const ivec2&, const ivec2& );
        virtual void draw( guiFramebuffer& );


        struct iterator
        {
            guiElement *curr;
        
            iterator( guiElement *E ): curr(E) {  };
            iterator( const iterator &it ): curr(it.curr) {  };
        
            iterator &operator++()
            {
                curr = curr->m_next;
                return *this;
            };
        
            iterator operator++(int)
            {
                return iterator(curr->m_next);
            };
        
            bool operator == ( const iterator &rhs ) { return curr == rhs.curr; };
            bool operator != ( const iterator &rhs ) { return curr != rhs.curr; };
            guiElement *&operator*() { return curr; };
        
        };

        iterator begin() { return iterator(m_child); };
        iterator end()   { return iterator(nullptr); };
    };


    // void update( guiElement* );
    // void draw( guiFramebuffer&, guiElement* );
}






// struct wm::guiElement::iterator
// {
//     guiElement *curr;

//     iterator( guiElement *E ): curr(E) {  };
//     iterator( const iterator &it ): curr(it.curr) {  };

//     iterator &operator++()
//     {
//         curr = curr->m_next;
//         return *this;
//     };

//     iterator operator++(int)
//     {
//         return iterator(curr->m_next);
//     };

//     bool operator == ( const iterator &rhs ) { return curr == rhs.curr; };
//     bool operator != ( const iterator &rhs ) { return curr != rhs.curr; };
//     guiElement *&operator*() { return curr; };

// };

