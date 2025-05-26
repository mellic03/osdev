#include <kernel/tty.hpp>
#include <stdint.h>
#include <kmemxx.hpp>


knlTTY::knlTTY()
{
    clear();
}

void knlTTY::clear()
{
    idx = 0;
    kmemset<uint8_t>(buf, '\0', sizeof(buf));
}

void knlTTY::putch( char ch )
{
    if (ch == '\b')
    {
        buf[--idx] = '\0';
    }

    else if (ch == '\t')
    {
        buf[idx++] = ' ';
        while (idx%4 != 0)
            buf[idx++] = ' ';
    }

    else if ((' ' <= ch && ch <= '~') || ch == '\n')
    {
        buf[idx++] = ch;
    }

    if (idx >= 1024)
    {
        idx = 0;
    }
}

void knlTTY::putstr( const char *str )
{
    while (*str)
        putch(*(str++));
}
