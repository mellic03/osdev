#include <libc.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


static ksysc_request req;
static ksysc_window_response res;

int main( int argc, int64_t *argv )
{
    if (argc || argv)
    {

    }

    req.type = SYSC_WINDOW_CREATE;
    libk_syscall(&req);
    memcpy(&res, req.res, sizeof(ksysc_window_response));

    req.type = SYSC_FILE_CREATE;
    req.size = 1024;
    strcpy(req.msg, "/env/sde.txt");
    libk_syscall(&req);

    return 0;
}
