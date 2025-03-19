#pragma once
#include <kdef.h>
#include <stim/interrupt.h>
#include <stim/syscall.h>



namespace idk
{
    void syscall_handler( kstackframe* );
    
}


