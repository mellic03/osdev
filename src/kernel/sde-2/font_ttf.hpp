#pragma once
#include <kpanic.h>
#include <kassert.h>
#include <kmalloc.h>
#include <math.h>
#include <string.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_ifloor(x)   ((int) floor(x))
#define STBTT_iceil(x)    ((int) ceil(x))
#define STBTT_sqrt(x)      sqrt(x)
#define STBTT_pow(x,y)     pow(x,y)
#define STBTT_fmod(x,y)    fmod(x,y)
#define STBTT_cos(x)       cos(x)
#define STBTT_acos(x)      acos(x)
#define STBTT_fabs(x)      fabs(x)
#define STBTT_malloc(x,u)  ((void)(u), kmalloc(x))
#define STBTT_free(x,u)    ((void)(u), kfree(x))
#define STBTT_assert(x)    kassert("STBTT_assert", x)
#define STBTT_strlen(x)    strlen(x)
#define STBTT_memcpy       memcpy
#define STBTT_memset       memset

#include <stb_truetype/stb_truetype.h>

// #include <kernel/vfs2.hpp>

// void YippeeExampleTTF()
// {
//     auto *fh = vfs2::find("/font/arial.ttf");

//     stbtt_fontinfo font;
//     stbtt_InitFont(&font, fh->sof, 0);
//     float scale = stbtt_ScaleForPixelHeight(&font, 15);

//     stbtt__bitmap dst = {1280, 720, 4, kvideo::backbuffer};

//     const char *str = "test";

//     while (*str)
//     {
//         stbtt_MakeCodepointBitmapSubpixel(
//             &font,
//             kvideo::backbuffer,
//             kvideo::W, kvideo::H, kvideo::stride,
//             1.0f, 1.0f, 0.0f, 0.0f,
//             *str
//         );

//         str++;
//     }

// }