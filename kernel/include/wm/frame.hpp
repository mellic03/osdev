#pragma once
#include "element.hpp"


namespace wm
{
    // struct guiContainer: public guiElement
    // {
    //     guiContainer( const ivec2 &tl, const ivec2 &sp )
    //     :   guiElement(tl, sp)
    //     {
    //     }
    
    //     virtual void draw( guiFramebuffer& ) override;
    // };

    struct guiWindow: public guiElement
    {
        // guiElement *m_focus = nullptr;
        using guiElement::guiElement;

        virtual void onMouseMove( guiMouse& ) override;
        // virtual void onClick( const ivec2& ) override;
        virtual void draw( guiFramebuffer& ) override;
    };


    struct guiRoot: public guiWindow
    {
        using guiWindow::guiWindow;

        virtual void onMouseMove( guiMouse& ) override;
        virtual void onClick( guiMouse& ) override;
        virtual void draw( guiFramebuffer& ) override;
    };
}


// #include <cringe/vec.hpp>
// #include <kmemxx.hpp>
// #include <string.h>
// // #include <functional>

// struct guiFramebuffer;


// struct guiContainer
// {
//     ivec2 tl, sp;
//     ivec4 margin;
//     void (*onHover)(guiContainer*) = nullptr;
//     void (*onClick)(guiContainer*) = nullptr;

//     guiContainer( const ivec2 &cnr=ivec2(0), const ivec2 &spn=ivec2(256),
//                   const ivec4 &mgn=ivec4(15) );

//     // guiContainer( int x, int y, int w, int h );

//     virtual void draw( guiFramebuffer* ) {  };

//     int xmin() const { return tl.x        + margin[0]; }
//     int xmax() const { return tl.x + sp.x - margin[1]; }
//     int ymin() const { return tl.y        + margin[2]; }
//     int ymax() const { return tl.y + sp.y - margin[3]; }
// };


// struct guiButton: public guiContainer
// {
//     char label[32];

//     guiButton( const char *name, const ivec2 &cnr, const ivec2 &spn );
//     virtual void draw( guiFramebuffer* ) final;

// };


// struct guiTextArea: public guiContainer
// {
//     char buf[1024];
//     int idx;
//     int vscroll;
//     u8vec4 color;

//     guiTextArea( const ivec2 &cnr=ivec2(0), const ivec2 &spn=ivec2(256),
//                  const ivec4 &mgn=ivec4(15) );

//     virtual void putch( char );
//     void putstr( const char* );

//     virtual void draw( guiFramebuffer* ) override;
// };




// struct guiTextInput: public guiTextArea
// {
//     // std::function<void()> onSubmit = [](){};
//     void (*submit)(guiTextInput*) = nullptr;

//     // using guiTextArea::guiTextArea;
//     guiTextInput( const ivec2 &cnr=ivec2(0), const ivec2 &spn=ivec2(256),
//                   const ivec4 &mgn=ivec4(15) )
//     : guiTextArea(cnr, spn, mgn)
//     {
//         color = ivec4(75);
//     }

//     virtual void putch( char ) final;
//     virtual void draw( guiFramebuffer* ) final;

// };

