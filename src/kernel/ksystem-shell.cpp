#include "./ksystem.hpp"
#include "driver/serial.hpp"
#include "driver/keyboard.hpp"
#include <kmalloc.h>
#include <kstring.h>
#include <kscancode.h>
#include <kfile.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <kfs.hpp>

using namespace idk;
using namespace KFS;


enum Command
{
    CMD_INVALID = 0,
    CMD_LSMEM,
    CMD_LSGFX,
    CMD_EXIT
};




Command next_cmd()
{
    Command cmd = CMD_INVALID;

    char buf[256];
    uint8_t idx = 0;
    memset(buf, '\0', sizeof(buf));

    while (true)
    {
        uint8_t key;

        if (KFile_read(&key, KFS::kdevscn, 1))
        {
            char ch = scode_getalpha(key);

            if (key == DOWN_ENTER)
            {
                break;
            }

            // else if (key == '\n')
            // {
            //     memset(buf, '\0', sizeof(buf));
            //     idx = 0;
            //     printf("\n");
            // }

            else if (ch)
            {
                ch = tolower(ch);
                buf[idx++] = ch;
                printf("%c", ch);
            }
        }
    }
    
    
    if (strncmp(buf, "lsmem", 5) == 0) return CMD_LSMEM;
    if (strncmp(buf, "lsgfx", 5) == 0) return CMD_LSGFX;
    if (strncmp(buf, "exit",  4) == 0) return CMD_EXIT;


    return CMD_INVALID;
}




void KSystem::shell( bool first )
{
    bool running = true;

    while (running)
    {
        printf(">> ");


        Command cmd = next_cmd();

        switch (cmd)
        {
            case CMD_INVALID: printf("\n  Invalid command"); break;
            case CMD_LSMEM:   printf("\n  --lsmem"); break;
            case CMD_LSGFX:   printf("\n  --lsgfx"); break;
            case CMD_EXIT:    running=false; break;
        }


        printf("\n");
    }

}


