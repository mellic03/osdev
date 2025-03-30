#include <libc.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main( int argc, int64_t *argv )
{
    if (argc || argv)
    {

    }

    ksysc_request req = {
        .type = SYSC_WINDOW_CREATE
    };

    libk_syscall(&req);

    ksysc_window_response res;
    memcpy(&res, req.res, sizeof(ksysc_window_response));

    while (*(res.x) < 250)
    {
        printf("res.x: %d\n", *(res.x));
        *(res.x) += 1;
    }

    // req = {
    //     .type = SYSC_WINDOW_DELETE,
    //     .data = res.sde_ctx
    // };
    // libk_syscall(&req);

    return 1234;
}
