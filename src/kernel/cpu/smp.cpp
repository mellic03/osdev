#include "../boot/boot.hpp"
#include "smp.hpp"
#include "ipc.hpp"
#include <mutex>
#include <atomic>

#include <kdef.h>
#include <kernel/log.hpp>
#include <stdio.h>
#include <string>


static std::atomic_uint64_t counter;

// static void hell( uint64_t *id )
// {
//     while (true)
//     {
//         counter++;
//         syslog::kprintf("[hell] cpu%u, counter=%u\n", *id, counter.load());
//     }
//     while (true) { asm volatile ("cli; hlt"); }
// }

// void server_main( void* )
// {
//     static uint8_t pkt;

//     ipcport_open(0x0008);

//     while (true)
//     {
//         if (ipcport_recv(0x0008, &pkt, 1, 1))
//         {
//             syslog::kprintf("[server_main] recieved %u\n", pkt);
//         }
//     }

// }

void SMP::init( limine_mp_response *res )
{
    syslog log0("SMP::init");

    // static uint64_t ids[4] = {0, 1, 2, 3};
    counter.store(0);

    for (size_t i=0; i<res->cpu_count; i++)
    {
        auto *mpinfo = res->cpus[i];

        syslog log("CPU %d", i);
        log("processor_id: %u", mpinfo->processor_id);
        log("lapic_id:     %u", mpinfo->lapic_id);
        log("goto_address: 0x%lx", mpinfo->goto_address);

        // mpinfo->extra_argument = (uint64_t)(&ids[i]);
        // mpinfo->goto_address = (limine_goto_address)&test;
    }

    // res->cpus[3]->extra_argument = (uint64_t)(&ids[3]);
    // __atomic_store_n(&(res->cpus[3]->goto_address), (limine_goto_address)&server_main, __ATOMIC_SEQ_CST);

}


