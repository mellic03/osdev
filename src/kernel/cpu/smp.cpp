#include "../boot/boot.hpp"
#include "smp.hpp"
#include "cpu.hpp"
#include "ipc.hpp"
#include "../gx/gx.hpp"

#include <cpuid.h>
#include <new>
#include <mutex>
#include <atomic>

#include <kdef.h>
#include <kfstream.hpp>
#include <kernel/log.hpp>
#include <stdio.h>
#include <string>


// static std::atomic_uint64_t counter;
// static uint8_t bufs1[4][128];
// static uint8_t bufs2[4][256];
// kfstream *cpu_streams[4];

uint32_t SMP::get_lapic_id()
{
    int cpu_info[4];
    __cpuidex(cpu_info, 1, 0); // Use CPUID leaf 1
    return (cpu_info[1] >> 24) & 0xFF; // LAPIC ID is in EBX[31:24]
}


// static void hell( limine_mp_info *info )
// {
//     uint64_t id = info->extra_argument;
//     processors[id].init();

//     syslog::kprintf("hell %lu, %u\n", id, SMP::get_lapic_id());

//     // cpu_streams[id] = new (bufs1[id]) kfstream(bufs2[id], 256, nullptr);

//     // while (true)
//     // {
//     //     uint64_t value = counter.fetch_add(1);
//     //     cpu_streams[id]->write(&value, sizeof(uint64_t));
//     // }

//     while (true) { asm volatile ("cli; hlt"); }
// }


// void SMP::init( limine_mp_response *res )
// {
//     syslog log0("SMP::init");
//     counter.store(0);

//     uint32_t boot_lapic = get_lapic_id();
//     log0("Boot lapic: %u", boot_lapic);

//     bool first = true;

//     for (size_t i=0; i<res->cpu_count; i++)
//     {
//         auto *mpinfo = res->cpus[i];

//         processors[i].core_number = i;
//         processors[i].enabled = false;

//         syslog log("CPU %d", i);
//         log("processor_id: %u", mpinfo->processor_id);
//         log("lapic_id:     %u", mpinfo->lapic_id);
//         log("goto_address: 0x%lx", mpinfo->goto_address);
//         mpinfo->extra_argument = (uint64_t)(i);

//         if (i != boot_lapic && first)
//         {
//             first = false;
//             __atomic_store_n(&(mpinfo->goto_address), &hell, __ATOMIC_SEQ_CST);
//         }
//     }
//     // kernel::cpu0->init();
// }


