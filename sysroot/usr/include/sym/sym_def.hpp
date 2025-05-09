#pragma once
#include <kdef.h>
#include <functional>

namespace ksym
{
    struct ksym_t
    {
        struct {
            void  (*assert)(bool);
            void  (*printf)(const char*, ...);
            void *(*malloc)(size_t);
            void *(*realloc)(void*, size_t);
            void  (*free)(void*);
            uint64_t (*clock)();
        } libc_sym;

        struct {
            uint8_t *frontbuffer;
            uint8_t *backbuffer;
            void (*fill)(uint8_t, uint8_t, uint8_t, uint8_t);
            void (*swapBuffers)();
        } kvideo_sym;

        struct {
            void (*yield)();
            void (*sleep)(uint64_t);
            void (*exit)();
        } kthread_sym;

        struct {
            void (*panic)(const char*);
            void (*hang)();
        } kernel_sym;
    };

    ksym_t *getsym();
    void    loadsym( ksym_t* );
}


