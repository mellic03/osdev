#pragma once
#include <kdef.h>
#include <cringe/vec.hpp>

struct vfsNode;
struct ModuleInterface;
struct kthread_t;


namespace knl
{
    struct MsState;
    struct MsEvent;
    struct KbEvent;
}




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
            vfsNode *(*open)(const char*);
            size_t   (*read)(vfsNode *fh, void *dst, size_t offset, size_t nbytes);
            size_t   (*write)(vfsNode *fh, const void *src, size_t offset, size_t nbytes);
            size_t   (*tell)(vfsNode *fh);
            int      (*seek)(vfsNode*, int64_t offset, uint8_t mode );
        } vfs_sym;

        struct {
            kthread_t *(*create)( const char*, void(*)(void*), void* );
            void (*yield)();
            void (*sleep)(uint64_t);
            void (*exit)();
            uint64_t (*this_cpuid)();
        } thread_sym;

        // struct {
        //     void (*installISR)(uint8_t, isrHandlerFn);
        //     void (*installIRQ)(uint8_t, irqHandlerFn);
        // } cpu_sym;

        struct {
            void   (*panic)(const char*);
            void   (*hcf)();
            vfsNode* (*popen)(const char*, size_t);
            void*  (*fopen)(const char*);
            size_t (*fread)(void*, void*, size_t);
            size_t (*fwrite)(void*, const void*, size_t);
            ModuleInterface *(*findModule)(uint64_t, uint64_t);
        } kernel_sym;
    };

    ksym_t *getsym();
    void    loadsym( ksym_t* );
}


