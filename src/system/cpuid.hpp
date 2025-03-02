#pragma once
#include <cpuid.h>

#define CPUID_FEAT_EDX_APIC (1 << 9)


namespace ck
{
    /* Example: Get CPU's model number */
    static int get_model(void)
    {
        int ebx, unused;
        __cpuid(0, unused, ebx, unused, unused);
        return ebx;
    }


    /* Example: Check for builtin local APIC. */
    static int check_apic(void)
    {
        unsigned int eax, unused, edx;
        __get_cpuid(1, &eax, &unused, &unused, &edx);
        return edx & CPUID_FEAT_EDX_APIC;
    }
    
}

