#include <idk_kernel.h>
#include <idk_virtio.h>
#include "./idk_file.hpp"


static idk_FILE  kfiles[8] __attribute__((aligned (8)));
static idk_FILE *kstderr;
static idk_FILE *kstdin;
static idk_FILE *kstdout;


kret_t filesystem_init()
{
    kstderr = &kfiles[0];
    kstdin  = &kfiles[1];
    kstdout = &kfiles[2];

    outvirt8(KFILE_WRTE, KTRGT_STDERR);
    outvirt64(KFILE_WRTE, (uint64_t)(kstderr));

    outvirt8(KFILE_WRTE, KTRGT_STDIN);
    outvirt64(KFILE_WRTE, (uint64_t)(kstdin));

    outvirt8(KFILE_WRTE, KTRGT_STDOUT);
    outvirt64(KFILE_WRTE, (uint64_t)(kstdout));

    return KRET_SUCCESS;
}