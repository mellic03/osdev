#include "tty.hpp"
#include <ctype.h>
#include <string.h>



kn_TTY::kn_TTY()
// :   data(80*25, '\0')
:   row(0), col(0), csr(0)
{
    memset(data, '\0', 80*20);
}



void
kn_TTY::putchar( char ch )
{
    data[row][col++] = ch;

    if (col >= 25)
    {
        row++;
        col = 0;
    }

    if (row >= 80)
    {
        memset(data[0], 0, 80);
        row = 0;
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