#include "kfs.hpp"
#include "../driver/serial.hpp"
#include "../log/log.hpp"
#include <kernel.h>
#include <kfile.h>
#include <kinplace/inplace_vector.hpp>

using namespace idk;


static inplace_vector<KFile*> KFS_files;
KFile *KFS::kstdio[3];
KFile *KFS::kstderr;
KFile *KFS::kstdin;
KFile *KFS::kstdout;

KFile *KFS::kdevio[2];
KFile *KFS::kdevkey;
KFile *KFS::kdevscn;


void dummy_flush( KFile *fh )
{
    fh->read  = fh->base;
    fh->write = fh->base;
}


void kstdout_flush( KFile *fh )
{
    while (fh->read < fh->write)
    {
        IO::outb(IO::COM1, *(fh->read));
        fh->read += 1;
    }

    fh->read  = fh->base;
    fh->write = fh->base;
}


void KFS::init()
{
    syslog log("KFS::init");

    KFS_files = inplace_vector<KFile*>(KMalloc<KFile*>(128), 128);
    kstdio[0] = KFS::KFile_create(256, dummy_flush);
    kstdio[1] = KFS::KFile_create(256, dummy_flush);
    kstdio[2] = KFS::KFile_create(1024, kstdout_flush);
    kstderr = kstdio[0];
    kstdin  = kstdio[1];
    kstdin  = kstdio[2];

    kdevio[0] = KFS::KFile_create(64, dummy_flush);
    kdevio[1] = KFS::KFile_create(64, dummy_flush);
    kdevkey   = kdevio[0];
    kdevscn   = kdevio[1];

    log("KFS_files: %u", KFS_files.size());
    log("kstderr: 0x%lx", kstderr);
    log("kstdin:  0x%lx", kstdin);
    log("kstdout: 0x%lx", kstdout);

}



KFile *KFS::KFile_create( size_t nbytes, void (*fsh)(KFile*) )
{
    auto *fh    = KMalloc<KFile>(1);
    auto *base  = KMalloc<uint8_t>(512);

    *fh = {
        .fd     = 0,
        .flags  = 0,
        .status = 0,
        .size   = nbytes,

        .base   = base,
        .read   = base+0,
        .write  = base+0,
        .eof    = base+nbytes,

        .fsh    = (fsh == nullptr) ? dummy_flush : fsh
    };

    KFS_files.push_back(fh);
    return fh;
}








// // Need to map virtual device
// // Once mapped, it supplies libc with access to stderr, stdin, stdout etc.
// static virtiodev *kfs_dev;


// void kfs_recv( uint16_t p, uint64_t d, size_t nbytes )
// {
//     SYSLOG_BEGIN("kfs_recv");

//     SYSLOG("port: %lu",  p);
//     SYSLOG("data: %lu",  d);
//     SYSLOG("size: %luB", nbytes);

//     if (p != KFILE_READ)
//     {
//         SYSLOG_END();
//         return;
//     }


//     uint8_t *ports = kfs_dev->ports;

//     switch (d)
//     {
//         default: 
//             break;

//         case KTRGT_STDERR:
//             SYSLOG("KTRGT_STDERR, kstderr=0x%lx", kstderr);
//             kfs_dev->send(KFILE_READ, (uint64_t)kstderr, 8);
//             break;

//         case KTRGT_STDIN:
//             SYSLOG("KTRGT_STDIN, kstdin=0x%lx", kstdin);
//             kfs_dev->send(KFILE_READ, (uint64_t)kstdin, 8);
//             break;

//         case KTRGT_STDOUT:
//             SYSLOG("KTRGT_STDOUT, kstdout=0x%lx", kstdout);
//             kfs_dev->send(KFILE_READ, (uint64_t)kstdout, 8);
//             break;
//     }

//     SYSLOG_END();
// }


