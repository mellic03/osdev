#include "tty.hpp"
#include <kmalloc.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>


#include <kscancode.h>
#include <kthread.hpp>
#include <kernel/vfs.hpp>
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


kTTY::kTTY( size_t size )
:   cwd     (kfilesystem::vfsInsertDirectory("dev/tty0/")),
    history (new char[size]),
    htop    (history),
    hend    (history + size),
    prompt  (new char[80]),
    ptop    (prompt),
    pend    (prompt + size),
    font    (nullptr),
    running (true)
{
    // syslog log("kTTY::kTTY( size_t )");
    clear();
}


void kTTY::hclear()
{
    memset(history, '\0', hend-history);
    htop = history;
}

void kTTY::pclear()
{
    memset(prompt, '\0', pend-prompt);
    ptop = prompt;
}

void kTTY::clear()
{
    hclear();
    pclear();
}


void kTTY::_putc( char *&top, char *end, char ch )
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
}


void kTTY::_puts( char *&top, char *end, const char *str )
{
    while (*str && (top < end))
    {
        _putc(top, end, *(str++));
    }
}


void kTTY::moveCursor( int dir )
{
    if (dir < 0)
    {
        ptop = std::clamp(ptop-1, prompt, pend-1);

        return;
    }

    if (dir > 0)
    {
        ptop = std::clamp(ptop+1, prompt, pend-1);

        return;
    }

}



vfsDirEntry *&kTTY::getCWD()
{
    if (cwd == nullptr)
    {
        cwd = kfilesystem::vfsInsertDirectory("dev/tty0/");

    }

    return cwd;
}


