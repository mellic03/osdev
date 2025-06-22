#include <wm/text.hpp>
#include <wm/render.hpp>
#include <kernel/kstring.h>
#include <kernel/log.hpp>



wm::guiTextArea::guiTextArea( guiFont *font, const ivec2 &tl, const ivec2 &sp )
:   guiElement(tl, sp),
    m_idx(0) // , m_row(0), m_col(0)
{
    m_color = u8vec4(50, 50, 50, 255);
    m_font  = font;
    kmemset((uint8_t*)m_buf, 0, sizeof(m_buf));
}


void wm::guiTextArea::putch( char ch )
{
    char *base = m_buf;
    char *top  = m_buf + m_idx;
    char *end  = m_buf + sizeof(m_buf);

    if ((ch == '\b') && (top > base))
    {
        int row0 = (top - base) / BUF_W;
        *(--top) = '\0';
        int row1 = (top - base) / BUF_W;
    
        if (row1 < row0)
        {
            kmemset((uint8_t*)(m_buf) + BUF_W*row0, 0, BUF_W);
        }
    }

    // else if (ch == '\t')
    // {
    //     *(top++) = ' ';
    //     while ((top - base) % 4 != 0)
    //         *(top++) = ' ';
    // }

    else if (ch == '\n')
    {
        *top = ch;

        int row = (top - base)/BUF_W + 1;
        int col = 0;
        top = m_buf + BUF_W*row + col;
    }

    else if (' ' <= ch && ch <= '~')
    {
        *(top++) = ch;
    }

    if (top < end)
    {
        m_idx = top - base;
    }

    else
    {
        scrolln(1);
        top = base + m_idx;
    }

}

void wm::guiTextArea::scroll()
{
    // int curr_row = std::clamp(m_idx/BUF_W, (size_t)0, BUF_H-1);
    int curr_row = BUF_H-1;

    for (int i=0; i<curr_row; i++)
    {
        auto *dst = m_buf + BUF_W*(i+0);
        auto *src = m_buf + BUF_W*(i+1);
        kmemcpy(dst, src, BUF_W);
    }

    kmemset(m_buf + BUF_W*curr_row, 0, BUF_W);

    // curr_row = std::max(curr_row-1, 0);
    m_idx = BUF_W*curr_row;
}


void wm::guiTextArea::scrolln( int n )
{
    for (int i=0; i<n; i++)
    {
        scroll();
    }
}


void wm::guiTextArea::putstr( const char *str )
{
    while (*str)
        putch(*(str++));
}


void wm::guiTextArea::draw( guiFramebuffer &dst )
{
    guiRenderRect(dst, m_color, m_gpos, m_gsp);

    if (m_font)
    {
        ivec2 csr(0, 0);

        int curr_row = m_idx / BUF_W;
        for (int row=0; row<=curr_row; row++)
            csr = guiRenderFont(dst, m_gpos, m_gsp, *m_font, m_buf+row*BUF_W, csr);

        guiRenderFont(dst, m_gpos, m_gsp, *m_font, "_", csr);
    }
}


void wm::guiTextArea::onEvent( const char *data )
{
    putstr(data);
}



// void wm::guiTextTTY::draw( guiFramebuffer &dst )
// {
//     guiRenderRect(dst, m_color, m_gpos, m_gsp);

//     auto &buf  = m_tty->m_buf;
//     auto &bufw = m_tty->BUF_W;

//     if (m_font)
//     {
//         ivec2 csr(0, 0);

//         int curr_row = m_idx / bufw;
//         for (int row=0; row<=curr_row; row++)
//             csr = guiRenderFont(dst, m_gpos, m_gsp, *m_font, buf+row*bufw, csr);

//         guiRenderFont(dst, m_gpos, m_gsp, *m_font, "_", csr);
//     }
// }








wm::guiTextInput::guiTextInput( guiFont *font, const ivec2 &tl, const ivec2 &sp )
:   guiTextArea(font, tl, sp)
{
    m_color = ivec4(75);

    // this->onKey = [](guiElement *el, const knl::KbEvent &ev ) {
    //     static_cast<guiTextInput*>(el)->putch(ev.key);
    // };
}


void wm::guiTextInput::putch( char ch )
{
    if (ch != '\n')
    {
        guiTextArea::putch(ch);
        return;
    }

    submit();
}


void wm::guiTextInput::submit()
{
    if (this->onSubmit != nullptr)
        this->onSubmit(this);
    memset(m_buf, '\0', m_idx);
    m_idx = 0;
}



void wm::guiTextInput::onKey( const knl::KbEvent &ev )
{
    putch(ev.key);
}


void wm::guiTextInput::draw( guiFramebuffer &dst )
{
    // guiElement::draw(dst);
    guiRenderRect(dst, m_color, m_gpos, m_gsp);

    if (m_font)
    {
        ivec2 csr = guiRenderFont(dst, m_gpos, m_gsp, *m_font, ">> ", m_gpos);
        csr = guiRenderFont(dst, m_gpos, m_gsp, *m_font, m_buf, csr);
        guiRenderFont(dst, m_gpos, m_gsp, *m_font, "_", csr);
    }
}


