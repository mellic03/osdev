// #include <kvirtio.h>
// #include <kmalloc.h>
// #include <driver/serial.hpp>

// #include <cstring>


// // static uint8_t kvirtualio[256];
// static constexpr size_t KPORT_MAX = 0xFFFF;
// static uint8_t    *kvirtualio;
// static virtiodev  *kvirtdevs;
// static virtiodev **kvirtdevmap;

// kret_t kvirtio_init()
// {
//     SYSLOG_BEGIN("kmalloc_init");

//     kvirtualio  = (uint8_t*)kmalloc(KPORT_MAX*sizeof(uint8_t));
//     kvirtdevs   = (virtiodev*)kmalloc(64*sizeof(virtiodev));
//     kvirtdevmap = (virtiodev**)kmalloc(KPORT_MAX*sizeof(virtiodev*));

//     for (size_t i=0; i<KPORT_MAX; i++)
//     {
//         kvirtdevmap[i] = nullptr;
//     }


//     SYSLOG("max port:  %u",    KPORT_MAX);
//     SYSLOG("base addr: 0x%lx", kvirtualio);
//     SYSLOG("end addr:  0x%lx", kvirtualio + KPORT_MAX);

//     SYSLOG_END();
//     return KRET_SUCCESS;
// }





// void virtiodev_send( uint16_t p, uint64_t q, size_t n )
// {
//     union {
//         uint64_t qword;
//         uint8_t  bytes[8];
//     };
//     qword = q;

//     for (size_t i=0; i<n; i++)
//     {
//         kvirtualio[p+i] = bytes[i];
//     }
// }


// virtiodev *virtio_map( uint16_t A, uint16_t B, void (*recv)(uint16_t, uint64_t, size_t) )
// {
//     SYSLOG_BEGIN("virtio_map");
//     SYSLOG("mapping device to range [0x%x, 0x%x]", A, B);

//     if ((A > B) || (A > KPORT_MAX) || (B > KPORT_MAX))
//     {
//         SYSLOG("failure");
//         SYSLOG_END();
//         return nullptr;
//     }

//     auto *dev = (kvirtdevs++);

//     *dev = {
//         .base  = A,
//         .end   = B,
//         .ports = kvirtualio,
//         .recv  = recv,
//         .send  = virtiodev_send
//     };

//     for (auto port=A; port<B; port++)
//     {
//         kvirtdevmap[port] = dev;
//     }

//     SYSLOG("success mapping device 0x%lx", dev);
//     SYSLOG_END();

//     return dev;
// }


// bool virtio_unmap( virtiodev *dev )
// {
//     auto A = dev->base;
//     auto B = dev->end;

//     SYSLOG_BEGIN("virtio_unmap");
//     SYSLOG("device:   0x%lx", dev);
//     SYSLOG("mapping: [0x%x, 0x%x]", A, B);

//     if ((A > B) || (A > KPORT_MAX) || (B > KPORT_MAX))
//     {
//         SYSLOG("failure");
//         SYSLOG_END();
//         return false;
//     }

//     for (auto port=A; port<B; port++)
//     {
//         kvirtdevmap[port] = nullptr;
//     }
    
//     SYSLOG("success");
//     SYSLOG_END();

//     return true;
// }






// void outvirt8( uint16_t p, uint8_t d )
// {
//     SYSLOG_BEGIN("outvirt8");
//     SYSLOG("port: 0x%x", p);
//     SYSLOG("byte: 0x%x", d);
//     kvirtualio[p] = d;

//     if (kvirtdevmap[p])
//     {
//         kvirtdevmap[p]->recv(p, d, 8);
//     }

//     SYSLOG_END();
// }

// uint64_t invirt64( uint16_t p )
// {
//     union {
//         uint64_t qword;
//         uint8_t  bytes[8];
//     };

//     for (int i=0; i<8; i++)
//     {
//         bytes[i] = kvirtualio[p+i];
//         kvirtualio[p+i] = 0;
//     }

//     return qword;
// }


