#include <idk_kernel.h>
#include <idk_virtio.h>
#include <kdriver/serial.hpp>
#include "./idk_file.hpp"


static idk_FILE  kfiles[8] __attribute__((aligned (8)));
static idk_FILE *kstderr;
static idk_FILE *kstdin;
static idk_FILE *kstdout;



// Need to map virtual device
// Once mapped, it supplies libc with access to stderr, stdin, stdout etc.
static virtiodev *kfs_dev;

void kfs_recv( uint16_t p, uint64_t d, size_t nbytes )
{
    SYSLOG_BEGIN("kfs_recv");

    SYSLOG("port=%lu",  p);
    SYSLOG("data=%lu",  d);
    SYSLOG("size=%luB", nbytes);

    if (p != KFILE_READ)
    {
        SYSLOG_END();
        return;
    }


    uint8_t *ports = kfs_dev->ports;

    switch (d)
    {
        default:           break;
        case KTRGT_STDERR: ports[KFILE_WRTE] = (uint64_t)kstderr; break;
        case KTRGT_STDIN:  ports[KFILE_WRTE] = (uint64_t)kstdin;  break;
        case KTRGT_STDOUT: ports[KFILE_WRTE] = (uint64_t)kstdout; break;
    }

    SYSLOG_END();
}




kret_t kfilesystem_init()
{
    kfs_dev = (virtiodev*)kmalloc(sizeof(virtiodev));

    *kfs_dev = {
        .base = 0xF000,
        .end  = 0xFA00,
        .recv = kfs_recv
    };

    virtio_map(kfs_dev);

    kstderr = &kfiles[0];
    kstdin  = &kfiles[1];
    kstdout = &kfiles[2];


    // outvirt8(KFILE_WRTE, KTRGT_STDERR);
    // outvirt64(KFILE_WRTE, (uint64_t)(kstderr));

    // outvirt8(KFILE_WRTE, KTRGT_STDIN);
    // outvirt64(KFILE_WRTE, (uint64_t)(kstdin));

    // outvirt8(KFILE_WRTE, KTRGT_STDOUT);
    // outvirt64(KFILE_WRTE, (uint64_t)(kstdout));

    return KRET_SUCCESS;
}

