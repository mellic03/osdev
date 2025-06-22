#pragma once

#include <stddef.h>
#include <stdint.h>


namespace knl
{
    class VmHeapAllocator
    {
    public:
        VmHeapAllocator( void *base, size_t size );
        void *alloc( size_t );
        void  free( void* );

    private:
        uint8_t *m_base;
        uint8_t *m_end;

        struct VmHeader
        {
            uint64_t  resv0 = 0x00000000;
            uint64_t  magic = 0xDEADBEBE;
            uint64_t  used  = 0;
            VmHeader *next = nullptr;
            uint8_t   data[];
        
            size_t avail()
            {
                return ((uint8_t*)next - (uint8_t*)this) - sizeof(VmHeader);
            }
        };

        VmHeader *split( VmHeader*, size_t );
    };

}


