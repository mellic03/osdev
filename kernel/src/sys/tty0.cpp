#include <sys/tty0.hpp>
#include <stdio.h>

knl::kTTY knl::tty0;
knl::EventEmitter<const char*> knl::tty0_stdout;