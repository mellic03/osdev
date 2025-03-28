#include "tty.hpp"
#include <kmalloc.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>


#include <kscancode.h>
#include <kproc.hpp>
#include "kfs/kfs.hpp"
#include "driver/keyboard.hpp"

#include "kshell/kshell.hpp"
#include <kmath/vec.hpp>
#include <stdio.h>



kTTY::kTTY()
:   history(nullptr), htop(nullptr), hend(nullptr),
    prompt(nullptr),  ptop(nullptr), pend(nullptr)
{

}


kTTY::kTTY( size_t size )
:   history ((char*)kmalloc(size)),
    htop    (history),
    hend    (history + size),
    prompt  ((char*)kmalloc(80)),
    ptop    (prompt),
    pend    (prompt + size)
{
    // cwd = KFS::insert("/home/tty0");
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
        cwd = KFS::insertDirectory("dev/tty0/");
        auto *A = KFS::insertFile("dev/tty0/", "kdevraw", (uintptr_t)KFS::kdevraw, 128);
        auto *B = KFS::insertFile("dev/tty0/", "kdevkey", (uintptr_t)KFS::kdevkey, 128);

        A->read  = &KFS::kdevraw->read;
        A->write = &KFS::kdevraw->write;
        B->read  = &KFS::kdevkey->read;
        B->write = &KFS::kdevkey->write;
    }

    return cwd;
}

// void kTTY::hputc( char ch )
// {
//     _putc(htop, hend, ch);
// }

// void kTTY::pputc( char ch )
// {
//     _putc(ptop, pend, ch);
// }


// const char*
// kTTY::submit()
// {
//     if (strlen(prompt) == 0)
//     {
//         return nullptr;
//     }

//     htop += sprintf(htop, " >> %s\n", prompt);
//     htop += sprintf(htop, "    ");
//     htop = kshell_interpret(htop, prompt, this);
//     *(htop++) = '\n';

//     memset(prompt, 0, 80);
//     ptop = prompt;

//     return nullptr;
// }


