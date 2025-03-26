#pragma once

#include <kdef.h>


struct KThread
{
    size_t tid;
    uint64_t rip, rsp, rdi, rbp;
};


void KThread_init();
void KThread_yield( KThread* );
KThread *KThread_new( void (*)(void*), void *arg );
void KThread_detach( KThread* );


// void KThread_init();
// void KThread_yield();
// void KThread_start();
