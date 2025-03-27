#include <stdio.h>
#include <kscancode.h>



int fgetc( FILE *fh )
{
    while (true)
    {
        char ch;

        if (KFile_read(&ch, fh, 1))
        {
            return int(ch);
        }
    }
    
}


