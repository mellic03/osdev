#pragma once
#include <stddef.h>
#include <stdint.h>
#include <sys/process.hpp>

struct cpu_t;


namespace SMP
{
    static constexpr size_t max_cpus = 8;
    extern size_t num_cpus;

    cpu_t     *get_cpu( uint32_t id );

    bool       is_bsp();
    uint64_t   bsp_id();

    cpu_t       *this_cpu();
    uint64_t     this_cpuid();
    knl::Sched  *this_sched();
    knl::Thread *this_thread();


    void yield();
    // thread_t  *createThread( process_t *P, void (*tmain)(void*), void *arg );
    // process_t *createProcess( const char *name, void (*pmain)(void*), void *arg );
}

