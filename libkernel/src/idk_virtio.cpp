#include <idk_virtio.h>
#include <idk_kret.h>
#include <kdriver/serial.hpp>

#include <cstring>


// static uint8_t kvirtualio[256];
static constexpr size_t KPORT_MAX = 0xFFFF;
static uint8_t   *kvirtualio;
static virtiodev **kvirtdevmap;

kret_t kvirtio_init()
{
    SYSLOG_BEGIN("kmalloc_init");

    kvirtualio = (uint8_t*)kmalloc(KPORT_MAX*sizeof(uint8_t));
    kvirtdevmap = (virtiodev**)kmalloc(KPORT_MAX*sizeof(virtiodev*));

    for (size_t i=0; i<KPORT_MAX; i++)
    {
        kvirtdevmap[i] = nullptr;
    }


    SYSLOG("max port:  %u",    KPORT_MAX);
    SYSLOG("base addr: 0x%lx", kvirtualio);
    SYSLOG("end addr:  0x%lx", kvirtualio + KPORT_MAX);

    SYSLOG_END();
    return KRET_SUCCESS;
}



bool virtio_map( virtiodev *dev )
{
    SYSLOG_BEGIN("virtio_map");

    auto A = dev->base;
    auto B = dev->end;

    SYSLOG("mapping device to range [0x%x, 0x%x]", A, B);

    if ((A > B) || (A > KPORT_MAX) || (B > KPORT_MAX))
    {
        SYSLOG("failure");
        SYSLOG_END();
        return false;
    }

    for (auto port=A; port<B; port++)
    {
        kvirtdevmap[port] = dev;
    }

    dev->ports = kvirtualio;

    SYSLOG("success");
    SYSLOG_END();

    return true;
}


bool virtio_unmap( virtiodev *dev )
{
    return false;
}





template <typename uintXX_t>
void outvirtn( uint16_t p, uintXX_t d )
{
    uint8_t *base = &kvirtualio[p];
    *((uintXX_t*)base) = d;

    if (kvirtdevmap[p])
    {
        kvirtdevmap[p]->recv(p, uint64_t(d), sizeof(uintXX_t));
    }
}


template <typename uintXX_t>
uintXX_t invirtn( uint16_t port )
{
    uintXX_t *base = (uintXX_t*)(&kvirtualio[port]);
    auto tmp = *base;
    *base = 0;
    return tmp;
}








#define LAZY_LMAO(__N__)\
void outvirt##__N__( uint16_t p, uint##__N__##_t d )\
{ outvirtn<uint##__N__##_t>(p, d); }\
uint##__N__##_t invirt##__N__( uint16_t p )\
{ return invirtn<uint##__N__##_t>(p); }\


extern "C"
{
    LAZY_LMAO(8);
    LAZY_LMAO(16);
    LAZY_LMAO(32);
    LAZY_LMAO(64);
}


