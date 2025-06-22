#pragma once
#include "element.hpp"
#include "image.hpp"
#include <cringe/vec.hpp>
#include <kmemxx.hpp>
#include <string.h>

#include <kernel/tty.hpp>
#include <sys/event.hpp>


namespace wm
{
    struct guiTextArea: public guiElement, public knl::EventListener<const char*>
    {
        static constexpr size_t BUF_W = 80; 
        static constexpr size_t BUF_H = 36; 
        char     m_buf[BUF_H*BUF_W];
        int      m_idx;
        guiFont *m_font;
        guiTextArea( guiFont *font, const ivec2&, const ivec2& );

        virtual void putch( char );
        void putstr( const char* );
        void scroll();
        void scrolln( int n );

        virtual void onEvent( const char* ) final;
        virtual void draw( guiFramebuffer& ) override;
    };


    // struct guiTextTTY: public guiTextArea
    // {
    //     knl::kTTY *m_tty;

    //     guiTextTTY( knl::kTTY *tty, guiFont *font, const ivec2 &tl, const ivec2 &sp )
    //     :  guiTextArea(font, tl, sp), m_tty(tty) {  };

    //     virtual void putch( char ch ) { m_tty->putch(ch); };
    //     void putstr( const char *str ) { m_tty->putstr(str); };
    //     void scroll() { m_tty->scroll(); };
    //     void scrolln( int n ) { m_tty->scrolln(n); };

    //     virtual void draw( guiFramebuffer& ) override;
    // };


    struct guiTextInput: public wm::guiTextArea
    {
        void (*onSubmit)(guiTextInput*) = nullptr;
        guiTextInput( guiFont *font, const ivec2&, const ivec2& );

        void submit();
        virtual void putch( char ) final;
    
        virtual void onKey( const knl::KbEvent& ) override;
        virtual void draw( guiFramebuffer& ) override;
    };

}

