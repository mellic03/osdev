#include "tty.hpp"
#include <kmalloc.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>


#include <kscancode.h>
#include <kproc.hpp>
#include "kfs/kfs.hpp"
#include "driver/keyboard.hpp"

#include "kshell.hpp"
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




void kshell_main( void *arg )
{
    using namespace kdriver::ps2_kb;

    kTTY *tty = (kTTY*)arg;
    KeyEvent event;

    while (true)
    {
        size_t nbytes = 0;
        nbytes = KFile_read(&event, KFS::kdevkey, sizeof(KeyEvent));

        if (nbytes == sizeof(KeyEvent))
        {
            uint8_t mask = event.mask;
            uint8_t ch   = event.key;

            if (ch == '\0')
            {
                continue;
            }

            else if (ch == '\n')
            {
                tty->hputs(" >> ");

                if (tty->ptop == tty->prompt)
                {
                    tty->hputc('\n');
                    continue;
                }

                tty->hputs(tty->prompt);
                tty->hputc('\n');
                // tty->hputs("\n    ");
                kshell_interpret(tty);
                tty->pclear();
                // tty->submit();
            }

            else
            {
                tty->pputc(ch);
            }

        }
    }
}


