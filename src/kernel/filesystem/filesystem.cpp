#include <kernel.h>
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

    SYSLOG("port: %lu",  p);
    SYSLOG("data: %lu",  d);
    SYSLOG("size: %luB", nbytes);

    if (p != KFILE_READ)
    {
        SYSLOG_END();
        return;
    }


    uint8_t *ports = kfs_dev->ports;

    switch (d)
    {
        default: 
            break;

        case KTRGT_STDERR:
            SYSLOG("KTRGT_STDERR, kstderr=0x%lx", kstderr);
            kfs_dev->send(KFILE_READ, (uint64_t)kstderr, 8);
            break;

        case KTRGT_STDIN:
            SYSLOG("KTRGT_STDIN, kstdin=0x%lx", kstdin);
            kfs_dev->send(KFILE_READ, (uint64_t)kstdin, 8);
            break;

        case KTRGT_STDOUT:
            SYSLOG("KTRGT_STDOUT, kstdout=0x%lx", kstdout);
            kfs_dev->send(KFILE_READ, (uint64_t)kstdout, 8);
            break;
    }

    SYSLOG_END();
}




kret_t kfilesystem_init()
{
    SYSLOG_BEGIN("kfilesystem_init");

    kfs_dev = virtio_map(0xF000, 0xFFFF, kfs_recv);
    SYSLOG("kfs_dev = 0x%lx", kfs_dev);

    kstderr = &kfiles[0];
    kstdin  = &kfiles[1];
    kstdout = &kfiles[2];


    // outvirt8(KFILE_WRTE, KTRGT_STDERR);
    // outvirt64(KFILE_WRTE, (uint64_t)(kstderr));

    // outvirt8(KFILE_WRTE, KTRGT_STDIN);
    // outvirt64(KFILE_WRTE, (uint64_t)(kstdin));

    // outvirt8(KFILE_WRTE, KTRGT_STDOUT);
    // outvirt64(KFILE_WRTE, (uint64_t)(kstdout));

    SYSLOG_END();
    return KRET_SUCCESS;
}

