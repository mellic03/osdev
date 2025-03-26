#include <stdio.h>
#include <kscancode.h>


static char scode_getalpha( uint8_t code )
{
    switch (code)
    {
        default: return '\0';
        case DOWN_Q: return 'Q';
        case DOWN_W: return 'W';
        case DOWN_E: return 'E';
        case DOWN_R: return 'R';
        case DOWN_T: return 'T';
        case DOWN_Y: return 'Y';
        case DOWN_U: return 'U';
        case DOWN_I: return 'I';
        case DOWN_O: return 'O';
        case DOWN_P: return 'P';
        case DOWN_A: return 'A';
        case DOWN_S: return 'S';
        case DOWN_D: return 'D';
        case DOWN_F: return 'F';
        case DOWN_G: return 'G';
        case DOWN_H: return 'H';
        case DOWN_J: return 'J';
        case DOWN_K: return 'K';
        case DOWN_L: return 'L';
        case DOWN_Z: return 'Z';
        case DOWN_X: return 'X';
        case DOWN_C: return 'C';
        case DOWN_V: return 'V';
        case DOWN_B: return 'B';
        case DOWN_N: return 'N';
        case DOWN_M: return 'M';
    }
}




int fgetc( FILE *fh )
{
    while (true)
    {
        uint8_t code;

        if (KFile_read(&code, fh, 1))
        {
            char ch = scode_getalpha(code);

            if (ch)
            {
                return int(ch);
            }
        }
    }
    
}


