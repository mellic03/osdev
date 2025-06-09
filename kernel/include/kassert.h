#pragma once
#include <kpanic.hpp>

void kernel_assert( bool cond, const char *msg, const char *file,
                    const char *func, int line );

#define kassert(cond) kernel_assert((cond), #cond, __FILE__, __func__, __LINE__)
