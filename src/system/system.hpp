#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "./syscall/syscall.hpp"
#include "./idk_sysio/idk_sysio.hpp"


namespace idk::sys
{
    int init( idk::base_allocator* );

}
