// #include "ksystem.hpp"
#include "kfs/kfs.hpp"
#include "tty.hpp"
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


enum Command: uint32_t
{
    CMD_INVALID = 0,
    CMD_LS,
    CMD_LSMEM,
    CMD_LSGFX,
    CMD_EXIT
};

struct Token
{
    uint32_t cmd;
    char args[4][32];

    Token()
    {
        cmd = CMD_INVALID;
        memset(args, '\0', sizeof(args));
    }
};

static kn_TTY   *kshell_tty;
static kfsEntry *kshell_cwd;
static KFile *kdevscn;
Token next_token();




void kshell( void* )
{
    bool running = true;
    static bool nline = true;

    while (running)
    {
        printf(">> ");
        Token T = next_token();

        switch (T.cmd)
        {
            case CMD_INVALID: printf("\n  Invalid command"); break;
            case CMD_LSMEM:   printf("\n"); break;
            case CMD_LSGFX:   printf("\n  --lsgfx"); break;
            case CMD_EXIT:    running=false; break;
        }

        printf("\n");
    }

}







#include <kproc.hpp>



Token next_token()
{
    Token T;

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

        // Don't want to manually call this
        kproc_yield();
    }
    
    
    if (strncmp(buf, "ls",    2) == 0) T.cmd = CMD_LS;
    if (strncmp(buf, "lsmem", 5) == 0) T.cmd = CMD_LSMEM;
    if (strncmp(buf, "lsgfx", 5) == 0) T.cmd = CMD_LSGFX;
    if (strncmp(buf, "exit",  4) == 0) T.cmd = CMD_EXIT;


    return T;
}


