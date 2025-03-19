#include "./idk_sysio.hpp"
#include "./idk_file.hpp"
#include "../drivers/serial.hpp"
#include "../io.hpp"
#include "bitmanip.hpp"

// #include "./sysio_syscall.hpp"
// #include "system/syscall/syscall.hpp"

#include <stdbool.h>
#include <stdio.h>
#include <cstring.hpp>


// extern "C"
// {
//     static uint8_t __ck_reserved[0x1000]
//     __attribute__((section("__ck_reserved"))); // 4096 bytes
// }

using iobuf_type = uint8_t[512];
static constexpr size_t NUM_IOFILES = 3;

static idk_FILE   *__iofiles;
static iobuf_type *__iobufs;

// struct idk_MappedIO
// {
//     idk_FILE iofiles[4];
//     uint8_t iobufs[4][512];
//     uint8_t sysports[256];
// };
// static idk_MappedIO *__MappedIO;


// void
// idk::sysio::__syscall_file_getstdio()
// {
//     // auto &req = *idk::internal::__sysreq;
//     auto &res = *((SysResponse_FILE*)idk::internal::__sysres);
//     serial_printf("[__syscall_file_getstdio]\n");

//     res.type = SysResponseType::SUCCESS;
//     res.addr = (uintptr_t)(__iofiles);
// }



void idk_sysio_init( idk::linear_allocator &mem )
{
    serial_printf("[idk_sysio_init]\n");

    static constexpr uint32_t flags[3] = {
        idk::FileFlag_STDERR, idk::FileFlag_STDIN, idk::FileFlag_STDOUT
    };

    __iofiles = mem.alloca<idk_FILE>(NUM_IOFILES);
    __iobufs  = mem.alloca<iobuf_type>(NUM_IOFILES);

    for (size_t i=0; i<NUM_IOFILES; i++)
    {
        __iofiles[i] = idk_FILE(
            __iobufs[i],
            __iobufs[i] + sizeof(__iobufs[i]),
            flags[i]
        );
    }

    __libc_stdio_init(
        (idk_file_t*)(__iofiles+0),
        (idk_file_t*)(__iofiles+1),
        (idk_file_t*)(__iofiles+2)
    );
}


bool idk_sysio_update()
{
    auto &stdout = __iofiles[2];

    if (stdout.status & idk::FileStatus_DIRTY)
    {
        auto &rbuf = stdout.buf;

        while (!rbuf.empty())
        {

        }

        stdout.status = idk::bit_unset(stdout.status, (uint32_t)idk::FileStatus_DIRTY);
        return true;
    }

    return false;
}



// void idk_writeport( uint8_t port, uint8_t byte )
// {
//     __MappedIO->sysports[port] = byte;
// }


// uint8_t idk_readport( uint8_t port )
// {
//     return __MappedIO->sysports[port];
// }

