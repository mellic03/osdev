#include "./ck_systemio.h"
#include "./ck_file.hpp"
#include "../drivers/serial.hpp"
#include "../io.hpp"
#include <stdbool.h>
#include <stdio.h>
#include <cstring.hpp>


extern "C"
{
    static uint8_t __ck_reserved[0x1000]
    __attribute__((section("__ck_reserved"))); // 4096 bytes

}


extern volatile char __ck_reserved_start;
extern volatile char __ck_reserved_start_phys;
extern volatile char __ck_reserved_end;




void flush_stdout2()
{
    uint8_t *start = stdout->head;
    uint8_t *end   = stdout->tail;

    for (uint8_t *str=start; str<end; str++)
    {
        ck::IO::outb(ck::serial::COM1, *str);
    }

    stdout->head = (stdout->data + 0);
    stdout->tail = (stdout->data + 0);
    stdout->status &= !ck::FileStatus_DIRTY;
}



bool ckSystemIO_update()
{
    if (stdout->status & ck::FileStatus_DIRTY)
    {
        // flush_stdout();
        stdout->status &= !ck::FileStatus_DIRTY;
        return true;
    }

    return false;

}



int ckSystemIO_init()
{
    uint32_t flags[4] = {
        ck::FileFlag_STDERR, ck::FileFlag_STDIN, ck::FileFlag_STDOUT
    };

    std::memset(__ck_reserved, 0, sizeof(__ck_reserved));
    auto &map = *((ckMappedIO*)__ck_reserved);

    for (int i=0; i<4; i++)
    {
        uint8_t *base = map.iobufs[i];

        map.iofiles[i] = {
            0, flags[i],
            base, base + sizeof(map.iobufs[i]),
            base, base,
            ck::FileStatus_GOOD
        };
    }

    __libc_stdio_init((void*)(__ck_reserved));

    return 1;
}




void ck_writeport( uint8_t port, uint8_t byte )
{
    ((ckMappedIO*)__ck_reserved)->sysports[port] = byte;
}


uint8_t ck_readport( uint8_t port )
{
    return ((ckMappedIO*)__ck_reserved)->sysports[port];
    // constexpr size_t budget = sizeof(__ck_reserved) - sizeof(ckMappedIO);
}



