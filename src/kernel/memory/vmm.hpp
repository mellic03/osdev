#pragma once

#include <stdint.h>
#include <stddef.h>


namespace VMM
{
    void init();
    void mapPage( uintptr_t phys, uintptr_t virt );
    void mapRange( uintptr_t phys, uintptr_t virt, size_t nbytes );
    void unmapPage( uintptr_t virt );
    // void unmapRange( uintptr_t virt, size_t nbytes );

}

