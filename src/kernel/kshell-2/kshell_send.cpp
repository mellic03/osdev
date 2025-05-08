// #include "kshell.hpp"
// #include <ipc.hpp>
// #include <kstring.h>
// #include <stdlib.h>

// using namespace KShell;


// char *kshell_send( char *dst, int argc, char argv[16][32]  )
// {
//     if (argc < 4)
//     {
//         dst = kssprintln(dst, "usage: send [port] [packet size] packets...");
//         return dst;
//     }

//     uint16_t port = uint16_t(atoi(argv[1]));
//     uint16_t psze = uint16_t(atoi(argv[2]));

//     for (int i=3; i<argc; i++)
//     {
//         if (psze == 1)
//         {
//             uint8_t pkt = uint8_t(atoi(argv[i]));
//             ipcport_send(port, &pkt, 1, psze);
//         }

//         if (psze == 2)
//         {
//             uint16_t pkt = uint16_t(atoi(argv[i]));
//             ipcport_send(port, &pkt, 1, psze);
//         }
    
//         if (psze == 4)
//         {
//             uint32_t pkt = uint32_t(atoi(argv[i]));
//             ipcport_send(port, &pkt, 1, psze);
//         }
    
//         if (psze == 8)
//         {
//             uint64_t pkt = uint64_t(atoi(argv[i]));
//             ipcport_send(port, &pkt, 1, psze);
//         }
//     }

//     return dst;
// }





// char *kshell_sendq( char *dst, int argc, char argv[16][32]  )
// {
//     if (argc < 3)
//     {
//         dst = kssprintln(dst, "usage: send [port] [packet size] packets...");
//         return dst;
//     }

//     static uint64_t buf[MAX_ARG_COUNT];
//     uint16_t port = uint16_t(atoi(argv[1]));

//     for (int i=2; i<argc; i++)
//         buf[i-2] = uint64_t(atoi(argv[i]));

//     ipcport_send(port, buf, argc-2, 8);

//     return dst;
// }




// char *kshell_sendmsg( char *dst, int argc, char argv[16][32]  )
// {
//     if (argc < 3)
//     {
//         dst = kssprintln(dst, "usage: send port byte");
//         return dst;
//     }

//     uint16_t port = uint16_t(atoi(argv[1]));
//     uint8_t  sot = 2;
//     uint8_t  eot = 3;
//     uint8_t  sep = ' ';

//     ipcport_send(port, &sot, 1, 1);
//     for (int i=2; i<argc; i++)
//     {
//         ipcport_send(port, argv[i], strlen(argv[i]), 1);
//         ipcport_send(port, &sep, 1, 1);
//     }
//     ipcport_send(port, &eot, 1, 1);

//     return dst;
// }


