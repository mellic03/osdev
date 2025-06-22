#include <kernel/memory/heap.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <util/bitmanip.hpp>
#include <sys/interrupt.hpp>
#include <kmemxx.hpp>
#include <kpanic.hpp>


knl::VmHeapAllocator::VmHeapAllocator( void *base, size_t size )
:   m_base ((uint8_t*)base),
    m_end  (m_base + size)
{
    *(VmHeader*)m_base = {
        .resv0 = 0x00000000,
        .magic = 0xDEADBEBE,
        .used  = 0,
        .next  = (VmHeader*)m_end
    };
}


knl::VmHeapAllocator::VmHeader*
knl::VmHeapAllocator::split( VmHeader *A, size_t Asz )
{
    uint8_t *Abase = (uint8_t*)A;
    uint8_t *Bbase = Abase + sizeof(VmHeader) + Asz;

    *(VmHeader*)(Bbase) = {
        .resv0 = 0x00000000,
        .magic = 0xDEADBEBE,
        .used  = 0,
        .next  = A->next
    };

    A->next = (VmHeader*)(Bbase);

    return A;
}


void *knl::VmHeapAllocator::alloc( size_t size )
{
    auto *A = (VmHeader*)m_base;
    size = idk::align_up(size, 128);

    while ((uint8_t*)A < m_end)
    {
        if ((A->used == 0) && (A->avail() > size))
        {
            A = this->split(A, size);
            A->used = 1;
            return (void*)(A->data);
        }
        
        A = A->next;
    }

    kpanic("[VmHeapAllocator::alloc] Bad alloc: A >= m_end");

    return nullptr;
}


void knl::VmHeapAllocator::free( void *ptr )
{
    auto *A = (VmHeader*)((uint8_t*)ptr - sizeof(VmHeader));
    auto *Abase = (uint8_t*)A;

    auto *B = A->next;
    auto *Bbase = (uint8_t*)B;

    if (!(m_base <= Abase && Abase < m_end))
        kpanic("[VmHeapAllocator::free] Bad free: !(m_base <= Abase && Abase < m_end)");

    if (A->magic != 0xDEADBEBE)
        kpanic("[VmHeapAllocator::free] Bad free: A->magic != 0xDEADBEBE");

    A->used = 0;

    if (Bbase < m_end && B->used == 0)
    {
        A->next = B->next;
        kmemset(B, 0, sizeof(VmHeader));
    }
}
