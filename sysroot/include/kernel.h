#pragma once

#include <kdef.h>
#include <kfile.h>
#include <kintcode.h>
#include <kmalloc.h>
#include <kstackframe.h>
#include <ksysc.h>
#include <kvirtio.h>


inline void kernel_hcf()
{
    while (1) { asm volatile ("cli; hlt"); }
}


