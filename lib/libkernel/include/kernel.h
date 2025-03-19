#pragma once
#include <AAA.h>

#include <kdef.h>
#include <kfile.h>
#include <kinterrupt.h>
#include <kmalloc.h>
#include <kvirtio.h>


inline void kernel_hcf()
{
    while (1) { asm volatile ("cli; hlt"); }
}


#include <AAB.h>
