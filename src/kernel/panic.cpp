#include "panic.hpp"
#include "kvideo/kvideo.hpp"
// #include "kvideo/font.hpp"
#include <kernel/log.hpp>
#include <kernel/vfs.hpp>
#include <kernel.h>
#include <kstackframe.h>


// static idk::FontBuffer *kpanic_font;

// void kpanic_init()
// {
//     auto *file = vfsFindFile("/font/cutive-w12hf18.bmp");
//     kpanic_font = new idk::FontBuffer((BMP_header*)(file->addr));
// }


void kpanic_handler( kstackframe *frame )
{
    kvideo::fill(vec3(0.8, 0.2, 0.2));
    kvideo::swapBuffers();

    const char *msg = (const char*)(frame->rdi);
    syslog log("KERNEL PANIC");
    log("reason: %s", msg);

    kernel_hcf();
}
