#pragma once
#include "AAA.h"
#include <kpanic.h>

void kernel_assert( const char *msg, bool cond,
                    const char *file, int line, const char *func );

#define kassert(msg, cond) kernel_assert(msg, cond, __FILE__, __LINE__, __func__)

#include "ZZZ.h"