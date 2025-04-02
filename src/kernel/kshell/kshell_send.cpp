#include "kshell.hpp"
#include <ipc.hpp>
#include <kstring.h>
#include <stdlib.h>

using namespace KShell;
extern kTTY *kshell_tty;



char *kshell_send( char *dst, int argc, char **argv )
{
    if (argc < 4)
    {
        dst = kssprintln(dst, "usage: send [port] [packet size] packets...");
        return dst;
    }

    uint16_t port = uint16_t(atoi(argv[1]));
    uint16_t psze = uint16_t(atoi(argv[2]));

    for (int i=3; i<argc; i++)
    {
        if (psze == 1)
        {
            uint8_t pkt = uint8_t(atoi(argv[i]));
            ipcport_send(port, &pkt, 1, psze);
        }

        if (psze == 2)
        {
            uint16_t pkt = uint16_t(atoi(argv[i]));
            ipcport_send(port, &pkt, 1, psze);
        }
    
        if (psze == 4)
        {
            uint32_t pkt = uint32_t(atoi(argv[i]));
            ipcport_send(port, &pkt, 1, psze);
        }
    
        if (psze == 8)
        {
            uint64_t pkt = uint64_t(atoi(argv[i]));
            ipcport_send(port, &pkt, 1, psze);
        }
    }

    return dst;
}




char *kshell_sendmsg( char *dst, int argc, char **argv )
{
    if (argc < 3)
    {
        dst = kssprintln(dst, "usage: send port byte");
        return dst;
    }

    uint16_t port = uint16_t(atoi(argv[1]));
    uint64_t startmsg = 0xDEADBEBE;
    uint8_t  endmsg   = '\0';
    uint8_t  sep  = ' ';

    ipcport_send(port, &startmsg, 1, 8);

    for (int i=2; i<argc; i++)
    {
        ipcport_send(port, argv[i], strlen(argv[i]), 1);
        ipcport_send(port, &sep, 1, 1);
    }
    ipcport_send(port, &endmsg, 1, 1);

    return dst;
}


