#pragma once
#include <kpanic.h>

void kernel_assert( const char *msg, bool cond, const char *file, int line, const char *func );

#define kassert(cond) kernel_assert(#cond, cond, __FILE__, __LINE__, __func__)
