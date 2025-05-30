#pragma once
#include "element.hpp"
#include "image.hpp"
#include <cringe/vec.hpp>
#include <kmemxx.hpp>
#include <string.h>
// #include <functional>



namespace wm
{
    struct guiTextArea: public guiElement
    {
        char     m_buf[1024];
        int      m_idx;
        guiFont *m_font;
        guiTextArea( guiFont *font, const ivec2&, const ivec2& );

        virtual void putch( char );
        void putstr( const char* );
        virtual void draw( guiFramebuffer& ) override;
    };

    struct guiTextArea2: public guiElement
    {
        char     m_buf[1024];
        int      m_row, m_col;
        guiFont *m_font;
        guiTextArea2( guiFont *font, const ivec2&, const ivec2& );

        virtual void putch( char );
        void putstr( const char* );

        virtual void onKey( const knl::KbEvent &ev ) override { putch(ev.key); }
        virtual void draw( guiFramebuffer& ) override;
    };


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

