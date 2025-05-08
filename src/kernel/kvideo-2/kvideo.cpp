// #ifndef __is_kernel
//     #define __is_kernel
// #endif

// #include "kvideo.hpp"

// #include <kernel/boot_limine.hpp>
// #include <kernel/log.hpp>
// #include <kmalloc.h>
// #include <kthread.hpp>
// #include <kmemxx.hpp>
// #include <string.h>
// #include <algorithm>


// int    kvideo::W = 0;
// int    kvideo::H = 0;
// int    kvideo::pitch = 0;
// size_t kvideo::BPP = 0;
// size_t kvideo::stride = 0;
// size_t kvideo::nbytes = 0;

// uint8_t* volatile kvideo::frontbuffer;
// uint8_t* volatile kvideo::backbuffer;


// void kvideo::initFrontbuffer( uintptr_t fbres )
// {
//     auto *res = (limine_framebuffer_response*)fbres;
//     // char buf[128];
//     // memset(buf, 0, 128);

//     // for (size_t i=0; i<res->framebuffer_count; i++)
//     // {
//     //     sprintf(buf, "/dev/fb%d", int(i));
//     //     auto *fh = vfsInsertFile<char>(buf, 256, vfsFileFlag_Text);
//     //     fh->flags |= vfsFileFlag_Text;
//     //     sprintf((char*)(fh->addr), "test content 1\ntest content 2\n");
//     // }

//     syslog log("kvideo::init");
//     auto *fb = res->framebuffers[0];

//     log("fb count:    %u", res->framebuffer_count);
//     log("width:       %u", fb->width);
//     log("height:      %u", fb->height);
//     log("pitch:       %u", fb->pitch);
//     log("bpp:         %u", fb->bpp);
//     log("pitch/w:     %u", fb->pitch / fb->width);
//     log("bpp/8:       %u", fb->bpp / 8);
//     log("mem model:   %u", fb->memory_model);
//     log("edid:        0x%lx", fb->edid);
//     log("rmask_size:  %u", fb->red_mask_size);
//     log("rmask_shift: %u", fb->red_mask_shift);
//     log("gmask_size:  %u", fb->green_mask_size);
//     log("gmask_shift: %u", fb->green_mask_shift);
//     log("bmask_size:  %u", fb->blue_mask_size);
//     log("bmask_shift: %u", fb->blue_mask_shift);

//     kvideo::W      = fb->width;
//     kvideo::H      = fb->height;
//     kvideo::pitch  = fb->pitch;
//     kvideo::BPP    = fb->bpp;
//     kvideo::stride = pitch/W;
//     kvideo::nbytes = pitch*H;

//     kvideo::frontbuffer = (uint8_t*)fb->address;
// }

// void kvideo::initBackbuffer( uintptr_t )
// {
//     kvideo::backbuffer = (uint8_t*)kmalloc(kvideo::nbytes);
// }


// void kvideo::swapBuffers()
// {
//     kmemcpy<uint8_t>(frontbuffer, backbuffer, nbytes);
// }



// void kvideo::fill( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
// {
//     static uint8_t rgba[4];
//     rgba[0] = b;
//     rgba[1] = g;
//     rgba[2] = r;
//     rgba[3] = a;

//     for (int i=0; i<W*H; i++)
//     {
//         for (size_t j=0; j<stride; j++)
//         {
//             frontbuffer[stride*i + j] = rgba[j];
//         }
//     }
// }

// void kvideo::fill( const vec4 &rgba )
// {
//     uint8_t r = uint8_t(255.0f * rgba.r);
//     uint8_t g = uint8_t(255.0f * rgba.g);
//     uint8_t b = uint8_t(255.0f * rgba.b);
//     uint8_t a = uint8_t(255.0f * rgba.a);
//     fill(r, g, b, a);
// }

// void kvideo::fill( const vec3 &rgb )
// {
//     fill(vec4(rgb, 1.0f));
// }

