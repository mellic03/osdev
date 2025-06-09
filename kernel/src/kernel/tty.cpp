#include <kernel/tty.hpp>
// #include <stdint.h>
// #include <kmemxx.hpp>



void knl::kTTY::putch( char ch )
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

void knl::kTTY::scroll()
{
    int curr_row = BUF_H-1;

    for (int i=0; i<curr_row; i++)
    {
        auto *dst = m_buf + BUF_W*(i+0);
        auto *src = m_buf + BUF_W*(i+1);
        kmemcpy(dst, src, BUF_W);
    }

    kmemset(m_buf + BUF_W*curr_row, 0, BUF_W);
    m_idx = BUF_W*curr_row;
}

void knl::kTTY::scrolln( int n )
{
    for (int i=0; i<n; i++)
    {
        scroll();
    }
}

