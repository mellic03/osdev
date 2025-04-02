#include "ipc.hpp"
#include <stdio.h>
#include <string.h>
#include <kmalloc.h>
#include <kthread.hpp>
#include <kernel/log.hpp>
#include <mutex>


io_head::io_head( RingBuffer &buf, io_head &otr )
:   B(buf), otr(otr), idx(0)
{

}

uint8_t*
io_head::operator++(int)
{
    size_t offset = idx*B.m_stride;
    idx = (idx+1) % B.m_cap;
    return B.base + offset;
}



RingBuffer::RingBuffer( size_t cap, size_t stride, void (*fn)() )
:   base  ((uint8_t*)kmalloc(cap*stride)),
    end   (base + cap*stride),
    read  (*this, write),
    write (*this, read),
    m_cap   (cap),
    m_size  (0),
    m_flsh  (fn)
{

    
}






// struct ipc_pctl
// {
//     uint16_t  base;
//     uint16_t  end;
//     uint8_t  *ports;

//     void (*recv)(uint16_t, uint64_t, size_t);
//     void (*send)(uint16_t, uint64_t, size_t);
// };

// bool ipc_mapctl( ipc_pctl* );
// bool ipc_unmapctl( ipc_pctl* );



#define MAX_PORT 0xFFFF
#define BUFFER_SIZE 4096


struct ipc_port
{
    int P;
    int owner_pid;

    uint8_t *buf;
    uint8_t *end;
    uint8_t  r=0;
    uint8_t  w=0;
    size_t   cap;
    int      size;

    std::mutex lock;

    ipc_port()
    {
        buf  = (uint8_t*)kmalloc(256);
        end  = buf + 256;
        r    = 0;
        w    = 0;
        cap  = 256;
        size = 0;
    }


    bool push( uint8_t value )
    {
    //     syslog log("ipc_port::push");
    //     log("r, w, sz: %u, %u", r, w, size);

        // if (w+1 == r)
        // {
        //     // syslog log("ipc_port::push failure");
        //     // log("fail");
        //     return false;
        // }

        buf[w++] = value;
        size++;
        // log("success");
        return true;
    }

    bool pop( uint8_t *value )
    {
        // syslog log("ipc_port::pop");
        // log("r, w, sz: %u, %u", r, w, size);

        if (r == w)
        {
            // log("fail");
            return false;
        }

        *value = buf[r++];
        size--;
        // log("success");
        return true;
    }

};


static int          test[4];
static ipc_port   *ipc_ports[MAX_PORT];
static std::mutex  global_lock;


__attribute__((constructor))
void ipcprt_init()
{
    test[0] = 0xD;
    test[1] = 0xE;
    test[2] = 0xA;
    test[3] = 0xD;

    for (int i=0; i<MAX_PORT; i++)
    {
        ipc_ports[i] = nullptr;
    }
}



ipcprt_status
ipcport_open( uint16_t P )
{
    if (P >= 0xFFFF)
    {
        return PORT_CLOSED;
    }

    if (ipc_ports[P] != nullptr)
    {
        return PORT_REFUSED;
    }

    ipc_ports[P] = new ipc_port;
    return PORT_OPEN;
}


ipcprt_status
ipcport_connect( uint16_t port )
{
    if (port >= 0xFFFF)
    {
        return PORT_CLOSED;
    }

    if (ipc_ports[port] == nullptr)
    {
        return PORT_CLOSED;
    }

    if (1) // (ipc_ports[port].accept(port) == true)
    {
        return PORT_ACCEPTED;
    }

    return PORT_REFUSED;
}



size_t ipcport_send( uint16_t P, const void *src, size_t packets, size_t pktsize )
{
    if (ipc_ports[P] == nullptr)
    {
        return 0;
    }

    ipc_port &port = *ipc_ports[P];
    port.lock.lock();
    auto  *srcbuf = (const uint8_t*)src;
    size_t total = 0;

    for (size_t i=0; i<packets; i++)
    {
        size_t n = 0;

        for (size_t j=0; j<pktsize; j++)
        {
            if (port.push(*(srcbuf++)) == false)
            {
                port.lock.unlock();
                return total;
            }
            n += 1;
        }

        total += n;
    }

    // kthread::yield();
    port.lock.unlock();

    return total;
}


size_t ipcport_recv( uint16_t P, void *dst, size_t packets, size_t pktsize )
{
    if (ipc_ports[P] == nullptr)
    {
        return 0;
    }

    // kthread::yield();
    ipc_port &port = *ipc_ports[P];
    port.lock.lock();

    uint8_t *dstbuf = (uint8_t*)dst;
    size_t   total = 0;

    for (size_t i=0; i<packets; i++)
    {
        size_t n = 0;

        for (size_t j=0; j<pktsize; j++)
        {
            if (port.pop(dstbuf+n) == false)
            {
                port.lock.unlock();
                return total;
            }
            n += 1;
        }

        total += n;
    }

    port.lock.unlock();
    return total;
}


