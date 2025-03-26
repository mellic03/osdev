#pragma once
#include <kdef.h>
#include "../interrupt/interrupt.hpp"


namespace idk
{
    void syscall_handler( kstackframe* );
}


