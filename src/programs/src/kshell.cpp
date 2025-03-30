// #include <libc.h>
#include <stdio.h>
// #include <string.h>
// #include <ctype.h>
// #include <algorithm>

// #include <kscancode.h>
// #include <kfile.h>
// #include <ksysc.h>


// enum Command
// {
//     CMD_INVALID = 0,
//     CMD_LSMEM,
//     CMD_LSGFX,
//     CMD_EXIT
// };


// char scode_getalpha( uint8_t code )
// {
//     switch (code)
//     {
//         default: return '\0';
//         case DOWN_Q: return 'Q';
//         case DOWN_W: return 'W';
//         case DOWN_E: return 'E';
//         case DOWN_R: return 'R';
//         case DOWN_T: return 'T';
//         case DOWN_Y: return 'Y';
//         case DOWN_U: return 'U';
//         case DOWN_I: return 'I';
//         case DOWN_O: return 'O';
//         case DOWN_P: return 'P';
//         case DOWN_A: return 'A';
//         case DOWN_S: return 'S';
//         case DOWN_D: return 'D';
//         case DOWN_F: return 'F';
//         case DOWN_G: return 'G';
//         case DOWN_H: return 'H';
//         case DOWN_J: return 'J';
//         case DOWN_K: return 'K';
//         case DOWN_L: return 'L';
//         case DOWN_Z: return 'Z';
//         case DOWN_X: return 'X';
//         case DOWN_C: return 'C';
//         case DOWN_V: return 'V';
//         case DOWN_B: return 'B';
//         case DOWN_N: return 'N';
//         case DOWN_M: return 'M';
//     }
// }



// static KFile *kdevscn;


// Command next_cmd()
// {
//     char buf[256];
//     uint8_t idx = 0;
//     memset(buf, '\0', sizeof(buf));

//     while (true)
//     {
//         uint8_t key;

//         if (KFile_read(&key, kdevscn, 1))
//         {
//             char ch = scode_getalpha(key);
        
//             if (key == DOWN_ENTER)
//             {
//                 break;
//             }

//             // else if (key == '\n')
//             // {
//             //     memset(buf, '\0', sizeof(buf));
//             //     idx = 0;
//             //     printf("\n");
//             // }

//             else if (ch)
//             {
//                 ch = tolower(ch);
//                 buf[idx++] = ch;
//                 printf("%c", ch);
//             }
//         }
//     }
    
    
//     if (strncmp(buf, "lsmem", 5) == 0) return CMD_LSMEM;
//     if (strncmp(buf, "lsgfx", 5) == 0) return CMD_LSGFX;
//     if (strncmp(buf, "exit",  4) == 0) return CMD_EXIT;


//     return CMD_INVALID;
// }



// static void tesfn( void* )
// {
//     while (true)
//     {
//         printf("[tesfn]\n");
//         kthread::yield();
//     }
    
// }


int main( int argc, int64_t *argv )
{
    if (argc || argv)
    {
        
    }

    // kthread::create(tesfn, nullptr);
    printf("[kshell] >> exit\n");


    return 0;
}
