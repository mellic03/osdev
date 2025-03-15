#pragma once

#include "system/memory/memory.hpp"
#include "system/data_structures/array.hpp"


namespace idk
{
    struct SysRequest;
    struct SysResponse;
}



// Request
// ---------------------------------------------------------------------------------------------
namespace idk
{
    static constexpr size_t SysRequest_MAX_BUFSIZE = 256;

    enum SysRequest_: uint32_t
    {
        SysRequest_NONE          = 0,
        SysRequest_FILE_CREATE   = 1,
        SysRequest_FILE_DELETE   = 2,
        SysRequest_FILE_RENAME   = 3,
        SysRequest_FILE_GETSTDIO = 4
    };

    struct SysRequestFlag
    {
        enum ____: uint32_t
        {
            NONE    = 0,
            SPECIAL = 1 << 1,
        };
    };

    struct SysRequest
    {
        uint32_t type;
        uint32_t flags;
        idk::array<char, SysRequest_MAX_BUFSIZE> buf;

        SysRequest( uint32_t type = SysRequest_NONE,
                    uint32_t flags = SysRequestFlag::NONE,
                    const char *buffer = "\0\0\0\0" );
    };
}
// ---------------------------------------------------------------------------------------------


// Response
// ---------------------------------------------------------------------------------------------
namespace idk
{
    static constexpr size_t SysResponse_MAX_BUFSIZE = 256;
    #define bsize SysResponse_MAX_BUFSIZE

    enum SysResponse_: uint32_t
    {
        SysResponse_NONE     = 0,
        SysResponse_FAILURE  = 1,
        SysResponse_SUCCESS  = 2
    };

    struct SysResponse
    {
        uint32_t type;
        uint32_t flags;

        union
        {
            uint64_t buf64[bsize/8];
            uint32_t buf32[bsize/4];
            uint16_t buf16[bsize/2];
            uint8_t  buf8[bsize];
        };

    } __attribute__ ((packed));


    struct SysFileResponse
    {
        uint32_t  type;
        uintptr_t addr;
        char msg[bsize - sizeof(uint64_t)];

    } __attribute__ ((packed));


    #undef bsize
}
// ---------------------------------------------------------------------------------------------




namespace idk
{
    // const SysResponse &Syscall( const SysRequest& );
    void Syscall( uint32_t *type, uint32_t *flags, char buf[256] );

    namespace internal
    {
        extern idk::SysRequest *__sysreq;
        extern idk::SysResponse *__sysres;
        void __syscall_handler( uint64_t );
        void __syscall_init( idk::linear_allocator& );
    }
}

