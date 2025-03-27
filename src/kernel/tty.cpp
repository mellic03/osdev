#include "tty.hpp"
#include <ctype.h>
#include <string.h>
#include <algorithm>



kn_TTY::kn_TTY()
:   W(80), H(25),
    row(0), col(0),
    CSR(0),
    dirty(false)
{
    memset(data, '\0', 80*20);
}


void
kn_TTY::setcursor( int csr )
{
    CSR = csr;
    row = CSR / W;
    col = CSR % W;
}


void
kn_TTY::setrow( int r )
{
    CSR = W*r + col;
}


void
kn_TTY::setcol( int c )
{
    CSR = W*row + c;
}



void
kn_TTY::movecursor( int dir )
{
    int r = row;

    CSR += dir;
    row = CSR / W;
    col = CSR % W;

    if (row < r)
    {
        for (int i=0; i<W; i++)
        {
            if (data[W*row+i] == '\0')
            {
                col = i;
                break;
            }
        }
    }

    CSR = std::clamp(W*row + col, 0, W*H);
}


void
kn_TTY::moverow( int dir )
{
    row = CSR / W;
    col = CSR % W;
    row += dir;
    CSR = W*row + col;
}


void
kn_TTY::movecol( int dir )
{
    row = CSR / W;
    col = CSR % W;
    col += dir;
    CSR = W*row + col;
}






void
kn_TTY::_shift_left( int r )
{
    int c = CSR % W;

    for (int i=c+1; i<W; i++)
    {
        data[W*r + i] = data[W*r + i+1];
    }
}

void
kn_TTY::_shift_right( int r )
{
    int c = CSR % W;

    for (int i=W-1; i>c; i--)
    {
        data[W*r + i] = data[W*r + i-1];
    }
}


void
kn_TTY::_insert( char c )
{
    _shift_right(CSR/W);
    data[CSR] = c;
    movecursor(1);
}



void
kn_TTY::backspace()
{
    _shift_left(CSR/W);
    data[CSR] = ' ';
    movecursor(-1);
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
        moverow(1);
        col = 0;
        CSR = W*row + col;
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

void tty_main( void *arg )
{
    auto &tty = *((kn_TTY*)arg);

    while (true)
    {
        char ch;

        if (KFile_read(&ch, KFS::kdevkey, 1))
        {
            // printf("tty.putchar(%c)\n", ch);
            tty.putchar(ch);
        }

        // printf("[tty_main]\n");
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


