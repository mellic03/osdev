#pragma once
#include <stddef.h>
#include <stdint.h>
#include <util/linkedlist.hpp>

enum VmFlag_: uint8_t
{
    VmFlag_Page4K = 0,
    VmFlag_Page2M = 1
};


struct VmAddressRange: knl::LinkedListNode
{
    uint64_t vbase;
    uint64_t pbase;

    union {
        uint64_t qword;
        struct {
            uint8_t size : 1; // size==VmFlag_Page4K || size==VmFlag_Page2M
            uint8_t rest : 7;
        };
    } flags;
};


struct VmAllocator
{
public:
    uint64_t m_pml4;

    void *alloc( size_t nbytes );
    void  free( void *ptr );

private:
    knl::LinkedList<VmAddressRange> m_ranges;
};

