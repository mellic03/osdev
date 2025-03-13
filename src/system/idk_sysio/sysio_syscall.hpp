#pragma once

#include <stddef.h>
#include <stdint.h>
#include "./idk_file.h"
#include "system/memory/base_allocator.hpp"


namespace idk::sysio
{
    void __syscall_file_create();
    void __syscall_file_delete();
    void __syscall_file_rename();
}

