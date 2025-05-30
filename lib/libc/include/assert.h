#pragma once
#include <stdbool.h>

void libc_assert( bool cond, const char *msg, const char *file,
                  const char *func, int line );

#define assert(cond) libc_assert(cond, #cond, __FILE__, __func__, __LINE__)
