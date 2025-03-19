#include <stim/stim.h>


void stim_makefile( const char *path )
{
    ksysc_file_request req = {
        .type   = SYSC_FILE_CREATE,
        .nbytes = 0,
        .data   = NULL
    };

    stim_syscall((ksysc_request*)(&req));
}



void stim_writefile( const char *path, const void *data, uint32_t nbytes )
{
    ksysc_file_request req = {
        .type   = SYSC_FILE_WRITE,
        .nbytes = nbytes,
        .data   = data
    };

    stim_syscall((ksysc_request*)(&req));

}
