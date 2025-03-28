#include "kfs.hpp"
#include "trie.hpp"
#include "../driver/serial.hpp"
#include "../log/log.hpp"
#include <kinplace/inplace_vector.hpp>
#include <kernel.h>
#include <kfile.h>
#include <string.h>

using namespace idk;


static inplace_vector<KFile*> KFS_files;
KFile *KFS::kstdio[3];
KFile *KFS::kstderr;
KFile *KFS::kstdin;
KFile *KFS::kstdout;

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


#include "../ksystem.hpp"
#include "../boot/boot.hpp"
#include <stdio.h>

static KFS::Trie fs_trie;

void KFS::init( uintptr_t sys )
{
    syslog log("KFS::init");

    fs_trie = KFS::Trie();

    KFS_files = inplace_vector<KFile*>(KMalloc<KFile*>(128), 128);
    kstdio[0] = KFS::KFile_create(256, dummy_flush);
    kstdio[1] = KFS::KFile_create(256, dummy_flush);
    kstdio[2] = KFS::KFile_create(1024, kstdout_flush);
    kstderr = kstdio[0];
    kstdin  = kstdio[1];
    kstdin  = kstdio[2];

    log("KFS_files: %u", KFS_files.size());
    log("kstderr: 0x%lx", kstderr);
    log("kstdin:  0x%lx", kstdin);
    log("kstdout: 0x%lx", kstdout);

    auto &system = *(idk::KSystem*)sys;

    // KFS::insertDirectory("bin/");
    // KFS::insertDirectory("fonts/");


}






vfsFileEntry *KFS::insertFile( const char *dir, const char *name,
                               void *ad, size_t sz, uint8_t type )
{
    return fs_trie.insertFile(dir, name, ad, sz, type);
}

vfsFileEntry *KFS::insertFile( const char *dir, const char *name, kfstream *stream )
{
    return fs_trie.insertFile(dir, name, stream);
}

vfsFileEntry *KFS::findFile( const char *fname )
{
    kthread::global_lock();
    vfsFileEntry *entry = fs_trie.findFile(fname);
    kthread::global_unlock();
    return entry;
}

vfsFileEntry *KFS::findFile( vfsDirEntry *cwd, const char *fname )
{
    kthread::global_lock();
    vfsFileEntry *entry = fs_trie.findFile(cwd, fname);
    kthread::global_unlock();
    return entry;
}

vfsDirEntry *KFS::insertDirectory( const char *path )
{
    return fs_trie.insertDirectory(path);
}

vfsDirEntry *KFS::findDirectory( const char *path )
{
    return fs_trie.findDirectory(path);
}

vfsDirEntry *KFS::findDirectory( vfsDirEntry *cwd, const char *path )
{
    return fs_trie.findDirectory(cwd, path);
}






KFile *KFS::KFile_create( size_t nbytes, void (*fsh)(KFile*) )
{
    KFile   *fh   = (KFile*)kmalloc(1 * sizeof(KFile));
    uint8_t *base = (uint8_t*)kmalloc(nbytes * sizeof(uint8_t));

    fh->fd     = 0;
    fh->flags  = 0;
    fh->status = 0;
    fh->size   = nbytes;
    fh->base   = base;
    fh->read   = base;
    fh->write  = base;
    fh->eof    = base+nbytes;
    fh->lock   = { false };
    fh->fsh    = (fsh == nullptr) ? dummy_flush : fsh;

    KFS_files.push_back(fh);

    return KFS_files.back();
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


