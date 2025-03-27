#include "tty.hpp"
#include <ctype.h>
#include <string.h>
#include <algorithm>



kn_TTY::kn_TTY()
:   W(80), H(25),
    dirty(false)
{
    data_top = data + 0;
    data_end = data + 80*25;

    memset(data, '\0', 80*25);
    memset(prompt, '\0', sizeof(prompt));

    memcpy(prompt, ">> ", 3);
    prompt_csr = 3;

}


// void
// kn_TTY::setcursor( int csr )
// {
//     CSR = csr;
//     row = CSR / W;
//     col = CSR % W;
// }


// void
// kn_TTY::setrow( int r )
// {
//     CSR = W*r + col;
// }


// void
// kn_TTY::setcol( int c )
// {
//     CSR = W*row + c;
// }



void
kn_TTY::movecursor( int dir )
{
    // CSR += dir;

    // for (int i=0; i<W; i++)
    // {
    //     if (data[W*row+i] == '\0')
    //     {
    //         col = i;
    //         break;
    //     }
    // }

    prompt_csr = std::clamp(prompt_csr+dir, 0, W-1);
}


// void
// kn_TTY::moverow( int dir )
// {
//     row = CSR / W;
//     col = CSR % W;
//     row += dir;
//     CSR = W*row + col;
// }


// void
// kn_TTY::movecol( int dir )
// {
//     row = CSR / W;
//     col = CSR % W;
//     col += dir;
//     CSR = W*row + col;
// }






void
kn_TTY::_shift_left()
{
    for (int i=prompt_csr; i<W-1; i++)
    {
        prompt[i] = prompt[i+1];
    }
}

void
kn_TTY::_shift_right()
{
    for (int i=W-1; i>prompt_csr; i--)
    {
        prompt[i] = prompt[i-1];
    }
}


void
kn_TTY::_insert( char c )
{
    _shift_right();
    prompt[prompt_csr] = c;
    prompt_csr = std::min(prompt_csr+1, W-1);
}



void
kn_TTY::backspace()
{
    prompt_csr = std::max(prompt_csr-1, 3);
    prompt[prompt_csr] = ' ';
    _shift_left();
}


void
kn_TTY::enter()
{

}



void
kn_TTY::putchar( char ch )
{
    dirty = true;

    if (ch == 8)
    {
        backspace();
    }

    else if (ch == '\t')
    {
        putchar(' ');
        putchar(' ');
        // putchar(' ');
        // putchar(' ');
    }

    else if (ch == '\n')
    {
        memcpy(data_top, prompt, sizeof(prompt));
        memset(prompt, 0, sizeof(prompt));
        data_top += sizeof(prompt);

        memcpy(prompt, ">> ", 3);
        prompt_csr = 3;

        // moverow(1);
        // col = 0;
        // CSR = W*row + col;
    }

    else
    {
        _insert(ch);
    }

}









#include <kscancode.h>
#include <kproc.hpp>
#include "kfs/kfs.hpp"
#include "driver/keyboard.hpp"

#define TERM_BACKSP 8
#define TERM_TAB    9
#define TERM_ENTER  13


// void tty_main( void *arg )
// {
//     auto &tty = *((kn_TTY*)arg);

//     while (true)
//     {
//         char ch;

//         if (KFile_read(&ch, KFS::kdevkey, 1))
//         {
//             tty.putchar(ch);
//         }

//         kproc_yield();
//     }
// }


#include "kshell.hpp"
#include <kmath/vec.hpp>
#include <stdio.h>


void prompt_main( void *arg )
{
    u64vec2 data = *(u64vec2*)(arg);

    char *prompt  = (char*)(data[0]);
    char *ptop    = prompt;
    char *pend    = prompt + 80;

    char *history = (char*)(data[1]);
    char *htop    = history;
    char *hend    = history + 25*80;

    KShellCommand cmd;
    cmd.prompt = prompt;

    while (true)
    {
        char ch;

        if (KFile_read(&ch, KFS::kdevkey, 1))
        {
            if (ch == '\n')
            {
                if (strlen(prompt) == 0)
                {
                    continue;
                }

                htop += sprintf(htop, " >> %s\n", prompt);
                htop += sprintf(htop, "    ");
                htop = kshell_interpret(htop, prompt);
                *(htop++) = '\n';

                memset(prompt, 0, 80);
                ptop = prompt;
            }

            else if (ch == 8) // backspace
            {
                ptop = std::max(ptop-1, prompt);
                *ptop = '\0';
            }

            else
            {
                *ptop = ch;
                ptop = std::min(ptop+1, pend-1);
                *ptop = '\0';
            }
        }

        kproc_yield();
    }
}






#include <ctype.h>

void keyprocess_main( void *arg )
{
    auto &tty = *((kn_TTY*)arg);

    struct
    {
        uint8_t modifier;
        uint8_t code;
    } packet;


    char shift_table[256];
    memset(shift_table, '\0', sizeof(shift_table));

    char mod_0_9[] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
    memcpy(&(shift_table['0']), mod_0_9, sizeof(mod_0_9));

    for (int i=0; i<='z'-'a'; i++)
    {
        shift_table['a'+i] = toupper('a'+i);
    }

    shift_table['/'] = '?';
    shift_table['['] = '{';
    shift_table[']'] = '}';
    shift_table[';'] = ':';
    shift_table['\''] = '\"';
    shift_table[','] = '<';
    shift_table['.'] = '>';


    while (true)
    {
        packet = {0, 0};

        if (KFile_read(&packet, KFS::kdevscn, sizeof(packet)))
        {
            if (packet.modifier == MODIFIER_CURSOR)
            {
                if (packet.code == 0x4B)
                {
                    tty.movecursor(-1);
                }
                if (packet.code == 0x4D)
                {
                    tty.movecursor(+1);
                }

                continue;
            }

            char ch = scode_getchar(packet.code);
    
            if (ch == '\0')
            {
                continue;
            }

            if (packet.modifier & MODIFIER_SHIFT)
            {
                if (shift_table[ch])
                {
                    ch = shift_table[ch];
                }
            }

            KFile_write(KFS::kdevkey, &ch, 1);
        }

        // printf("[keyprocess_main]\n");
        kproc_yield();
    }
}


