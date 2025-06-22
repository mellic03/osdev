#pragma once

#include <kernel/tty.hpp>
#include <sys/event.hpp>


namespace knl
{
    extern knl::kTTY tty0;
    extern EventEmitter<const char*> tty0_stdout;

}


