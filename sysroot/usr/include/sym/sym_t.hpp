#pragma once
#include <kdef.h>
#include <functional>
#include <kernel/interrupt.hpp>
#include <kernel/input.hpp>
#include <cringe/vec.hpp>

struct ModuleInterface;


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
            void *(*fopen)(const char*);
        } libc_sym;

        struct {
            int W, H;
            uint8_t *frontbuffer;
            uint8_t *backbuffer;
            void (*clearBuffer)(uint8_t *buffer);
            void (*fillColor)  (uint8_t, uint8_t, uint8_t, uint8_t);
            void (*fillBuffer) (uint8_t *buffer);
            void (*rect)       (int, int, int, int);
            void (*blit)       (const ivec2&, uint8_t*, int, int, const ivec2&, const ivec2&);
            void (*swapBuffers)();
        } video_sym;

        struct {
            void (*yield)();
            void (*sleep)(uint64_t);
            void (*exit)();
        } thread_sym;

        struct {
            void (*triggerMouseEvent)(uint32_t, uint32_t);
            void (*writeMsData)(const kinput::MsData*);
            void (*readMsData)(kinput::MsData*);
        } input_sym;

        // struct {
        //     void (*installISR)(uint8_t, isrHandlerFn);
        //     void (*installIRQ)(uint8_t, irqHandlerFn);
        // } cpu_sym;

        struct {
            void (*panic)(const char*);
            void (*hcf)();
            ModuleInterface *(*findModule)(uint64_t, uint64_t);
        } kernel_sym;
    };

    ksym_t *getsym();
    void    loadsym( ksym_t* );
}


