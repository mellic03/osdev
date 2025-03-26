#include "frame_tty.hpp"
#include "../driver/keyboard.hpp"
#include "../kfs/kfs.hpp"
#include <stdio.h>


kwin::FrameTTY::FrameTTY( ivec2 tl, ivec2 sp )
:   Frame(tl, sp, uvec4(255)),
    m_row(0),
    m_col(0)
{
    memset(m_buf, '\0', sizeof(m_buf));

}


void
kwin::FrameTTY::draw( kwin::Context &ctx )
{
    char code;
    char ch = '\0';

    if (KFile_read(&code, KFS::kdevscn, 1))
    {
        ch = scode_getalpha(code);

        if (ch)
        {
            m_buf[80*m_row + m_col] = ch;
            m_col++;
        }
    }

    if (m_col >= 25)
    {
        m_row++;
        m_col = 0;
    }

    if (m_row >= 80)
    {
        // memset(data[0], 0, 80);
        m_row = 0;
    }


    ivec2 tl = m_tl;

    for (int c=0; c<m_col; c++)
    {
        tl.x += 15;
        ctx.rect(tl, ivec2(15, 25), uvec4(255));
        tl.x += 5;
    }

    ctx.rectOutline(m_tl, m_sp, m_col);
}


