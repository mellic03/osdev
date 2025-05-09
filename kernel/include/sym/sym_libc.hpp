#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>

#define FPTR inline idk::fptr

namespace std
{
    FPTR <void (bool)>              assert;
    FPTR <void (const char*, ...)>  printf;

    FPTR <void*(size_t)>            malloc;
    FPTR <void*(void*, size_t)>     realloc;
    FPTR <void (void*)>             free;

    FPTR <uint64_t()>               clock;
}

#undef FPTR

