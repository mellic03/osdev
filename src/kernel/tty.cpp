#include "tty.hpp"
#include <kmalloc.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>


#include <kernel/kscancode.h>
#include <kthread.hpp>
// #include <kernel/vfs.hpp>
#include "driver/keyboard.hpp"

#include "kshell/kshell.hpp"
#include <kmath/vec.hpp>
#include <stdio.h>



kTTY::kTTY()
:   cwd(nullptr),
    history(nullptr), htop(nullptr), hend(nullptr),
    prompt(nullptr),  ptop(nullptr), pend(nullptr),
    font(nullptr),
    running(true)
{
    // syslog log("kTTY::kTTY( void )");
}


kTTY::kTTY( int hist_len )
:   cwd     (vfs2::open<vfs2DirEntry>("dev/tty0/")),
    hlen    (hist_len),
    hrow    (0),
    size    (80*hist_len),

    history ((char*)kmalloc(size*sizeof(char))),
    hbase   (history),
    htop    (hbase),
    hend    (hbase + size),

    prompt  ((char*)kmalloc(80*sizeof(char))),
    pbase   (prompt),
    ptop    (pbase),
    pend    (pbase + size),

    response((char*)kmalloc(size*sizeof(char))),
    rbase   (response),
    rtop    (rbase),
    rend    (rbase + size),

    font    (nullptr),
    running (true)
{
    // syslog log("kTTY::kTTY( size_t )");
    clear();
}


void kTTY::_clear( char *&top, char *base )
{
    memset(base, '\0', top-base);
    top = base;
}


void kTTY::clear()
{
    hclear();
    pclear();
    rclear();
}


void kTTY::_putc( char *&top, char*, char *end, char ch )
{
    if (ch == 8) // backspace
    {
        top = std::max(top-1, prompt);
        *top = '\0';
    }

    else if (ch != '\0')
    {
        *top = ch;
        top = std::min(top+1, end-1);
        *top = '\0';
    }

    if (top == ptop)
    {
        *(ptop+0) = '_';
        *(ptop+1) = '\0';
    }
}


void kTTY::_putr()
{
    int col = 0;
    char *s = rbase;

    while (*s)
    {
        char ch = *(s++);

        if (ch == '\t')
        {
            int up = (col + 4-1) & ~(4-1);
            for (int i=0; i<up-col; i++)
            {
                hputc(' ');
            }
        }

        hputc(ch);

        if (ch == '\n')
            col = 0;
        else
            col++;
    }

}

void kTTY::submit( void (*callback)(kTTY*) )
{
    if (ptop > prompt)
    {
        *ptop = '\0'; // remove cursor
        hsprintf("%s\n", prompt);
        callback(this);

        if (rtop > rbase)
        {
            _putr();
        }
    }

    else
    {
        hputc('\n');
    }

    hsprintf("[%s] ", getCWD()->name);
    pclear();
    rclear();
}


void kTTY::hputc( char ch )
{
    _putc(htop, history, hend, ch);
}


void kTTY::_puts( char *&top, char *base, char *end, const char *str )
{
    while (*str && (top < end))
    {
        _putc(top, base, end, *(str++));
    }
}


void kTTY::hputs( const char *str )
{
    while (*str && (htop < hend))
    {
        hputc(*(str++));
    }
}


void kTTY::moveCursor( int dir )
{
    if (dir < 0)
    {
        ptop = std::clamp(ptop-1, prompt, pend-1);
    }

    else if (dir > 0)
    {
        ptop = std::clamp(ptop+1, prompt, pend-1);
    }

    *(ptop+1) = '_';
}



vfs2DirEntry *&kTTY::getCWD()
{
    if (cwd == nullptr)
    {
        cwd = vfs2::open<vfs2DirEntry>("dev/tty0/");
    }

    return cwd;
}


