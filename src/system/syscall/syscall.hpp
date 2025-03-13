#pragma once

#include "system/memory/base_allocator.hpp"
#include "system/data_structures/array.hpp"


namespace idk
{
    struct SysRequest;
    struct SysResponse;
    const SysResponse &syscall( const SysRequest& );

    namespace internal
    {
        extern idk::SysRequest *__sysreq;
        extern idk::SysResponse *__sysres;
        void __syscall_handler();
        void __syscall_init( idk::base_allocator* );
    }
}



// Request
// ---------------------------------------------------------------------------------------------
namespace idk
{
    static constexpr size_t SysRequest_MAX_BUFSIZE = 256;

    struct SysRequestType
    {
        enum ____: uint32_t
        {
            NONE        = 0,
            FILE_CREATE = 1,
            FILE_DELETE = 2,
            FILE_RENAME = 3,
        };
    };

    struct SysRequest
    {
        uint32_t type;
        idk::array<char, SysRequest_MAX_BUFSIZE> buf;
        SysRequest( uint32_t type = SysRequestType::NONE, const char *buffer = "\0\0\0\0" );
    };
}
// ---------------------------------------------------------------------------------------------


// Response
// ---------------------------------------------------------------------------------------------
namespace idk
{
    static constexpr size_t SysResponse_MAX_BUFSIZE = 256;
    #define bsize SysResponse_MAX_BUFSIZE

    struct SysResponseType
    {
        enum ____: uint32_t
        {
            NONE     = 0,
            FAILURE  = 1,
            SUCCESS  = 2
        };
    };

    struct SysResponse
    {
        uint32_t type;

        union
        {
            uint64_t buf64[bsize/8];
            uint32_t buf32[bsize/4];
            uint16_t buf16[bsize/2];
            uint8_t  buf8[bsize];
        };

    } __attribute__ ((packed));


    struct SysResponse_FILE
    {
        uint32_t type;
        uint64_t addr;
        char msg[bsize - sizeof(uint64_t)];

    } __attribute__ ((packed));


    #undef bsize
}
// ---------------------------------------------------------------------------------------------
