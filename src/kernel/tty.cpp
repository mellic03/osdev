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
kn_TTY::backspace()
{
    dirty = true;
    movecursor(-1);
    data[CSR] = '\0';
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
        data[CSR] = ch;
        movecursor(1);
    }

}



// void
// kn_TTY::update()
// {
//     uint8_t ch;

//     while (KFile_read(&ch, stream, 1))
//     {
//         if (isalpha(ch))
//         {
//             putchar(ch);
//         }
//     }
// }