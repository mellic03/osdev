#pragma once
#include <kdef.h>
#include <idk_fptr.hpp>

#define FPTR inline idk::fptr

namespace kthread
{
    FPTR <void()>         yield;
    FPTR <void(uint64_t)> sleep;
    FPTR <void()>         exit;
}

#undef FPTR



