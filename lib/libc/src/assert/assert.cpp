#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void libc_assert( bool cond, const char *msg, const char *file,
                  const char *func, int line )
{
    if (!cond)
    {
        fprintf(
            stderr,
            "assertion failed: (%s)\n\"%s\", line %d in function \"%s\"",
            msg, file, line, func
        );

        exit(1);
    }
}

