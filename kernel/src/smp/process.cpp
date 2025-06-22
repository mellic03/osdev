#include <sys/process.hpp>
#include <kernel/memory/pmm.hpp>
#include <kernel/memory/vmm.hpp>
#include <kernel/log.hpp>
#include <util/bitmanip.hpp>
#include <cpu/cpu.hpp>
#include <string.h>
#include <util/bitmap.hpp>

#include <kmalloc.h>
#include <new>


static idk::BitMapAllocator<knl::Process, knl::MAX_PROCESSES> ProcAllocator;
knl::LinkedList<knl::Process> knl::AllProcs;
knl::Process *knl::InitProc = nullptr;


knl::Process::Process( const char *name )
{
    static std::mutex M{0};
    static uint64_t curr_pid;

    memset(m_name, '\0', sizeof(m_name));
    strncpy(m_name, name, sizeof(m_name)-1);

    {
        std::lock_guard lock(M);
        m_pid = curr_pid++;
        knl::AllProcs.insert(this);
    }


    vmm_pml4_t tmp = { PMM::allocFrame() };
    vmm_pml4_t pml4 = { CPU::getCR3() };
    pml4.address = tmp.address;

    m_pml4 = pml4.qword;
    auto *ppml4 = (uint64_t*)(m_pml4 + PMM::hhdm);
    auto *kpml4 = (uint64_t*)(CPU::getCR3() + PMM::hhdm);

    VMM::mapPage(m_pml4, m_pml4 + PMM::hhdm);

    for (int i=0; i<256; i++)
        ppml4[i] = 0;

    for (int i=256; i<512; i++)
        ppml4[i] = kpml4[i];

    uintptr_t phys = PMM::allocPage();
    uintptr_t virt = 0x1000;
    VMM::mapPage2(m_pml4, phys, virt);

    // VMM::unmapPage(m_pml4 + PMM::hhdm);
    // CPU::setCR3((uint64_t)m_pml4);
    // CPU::setCR3((uintptr_t)kpml4 - PMM::hhdm);
};



knl::Process*
knl::createProcess( const char *name, void (*pmain)(void*), void *arg )
{
    Process *P  = ProcAllocator.alloc(name);
    knl::createThread(P, pmain, arg);
    return P;
}



knl::Thread*
knl::createThread( Process *P, void (*tmain)(void*), void *arg )
{
    static std::atomic_uint8_t idx{0};

    // auto *cpu = SMP::this_cpu();
    auto *cpu = SMP::get_cpu(idx++ % SMP::num_cpus);
    auto &sd  = cpu->sched2;
    auto *th  = sd.createThread(tmain, arg);

    if (P == nullptr)
        P = SMP::this_thread()->process;
    th->process = P;
    P->m_threads.insert(th);

    return th;
}


void knl::threadYield()
{
    knl::interrupt<IntNo_KThreadYield>();
}
