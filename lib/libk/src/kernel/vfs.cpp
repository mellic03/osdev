#include <kernel/vfs.hpp>
#include <kernel/vfspath.hpp>
#include <kernel/log.hpp>
#include <stdio.h>
#include <string.h>
#include <kmalloc.h>
#include <kstring.hpp>
#include <mutex>

// static char m_buf0[128];
// static char m_buf1[128];
vfsDirEntry *vfsEntry::rootdir = new vfsDirEntry(nullptr, "R:");
// static vfsDirEntry *m_root = new vfsDirEntry(m_root, "R");
static std::mutex vfs_mutex;



#define VFS_SYSLOG syslog syslogvfs
#define VFS_LOG syslogvfs



vfsDirEntry*
vfsInsertDirectory( const char *dpath )
{
    auto lock = kthread::yield_guard(kthread::lock_count);
    VFS_SYSLOG("vfsInsertDirectory");
    VFS_LOG("dpath:  \"%s\"", dpath);

    vfsDirEntry *dir = vfsEntry::rootdir;
    fs::directorypath path(dpath);

    for (auto &name: path.m_sep)
    {
        auto *child = dir->getChild<vfsEntry>(name.c_str());

        if (!child)
        {
            child = dir->giveChild<vfsDirEntry>(name.c_str());
        }

        else if (child->is_file())
        {
            VFS_LOG("Cannot create directory \"%s\". File with same name exists");
            return nullptr;
        }

        VFS_LOG("\"%s\" --> \"%s\"", dir->name, name.c_str());
        dir = static_cast<vfsDirEntry*>(child);
    }

    VFS_LOG("created directory \"%s\"", dir->get_path());

    return dir;
}


vfsDirEntry*
vfsFindDirectory( const char *pth )
{
    return vfsFindDirectory(vfsEntry::rootdir, pth);
}


vfsDirEntry*
vfsFindDirectory( vfsDirEntry *cwd, const char *dpath )
{
    auto lock = kthread::yield_guard(kthread::lock_count);
    VFS_SYSLOG("vfsFindDirectory(%s)", dpath);

    if ((strlen(dpath) == 1) && (dpath[0] == '/'))
        return vfsEntry::rootdir;
    if (dpath[0] == '/')
        cwd = vfsEntry::rootdir;

    fs::directorypath path(dpath);
    auto dirname = path.m_dirname;
    VFS_LOG("dirname: %s", dirname.c_str());

    vfsDirEntry *dir = cwd;

    for (auto &name: path.m_sep)
    {
        VFS_LOG("name: %s", name.c_str());
        
        if (name == ".")
        {
            continue; // dir =  dir;
        }

        else if (name == "..")
        {
            dir = (dir->parent) ? dir->parent : dir;
            continue;
        }

        vfsEntry *child = dir->getChild(name.c_str());

        if (!child)
        {
            VFS_LOG("no such directory \"%s%s\"", dir->get_path(), name.c_str());
            return nullptr;
        }
    
        else if (child->is_dir())
        {
            dir = static_cast<vfsDirEntry*>(child);
        }

        else
        {
            break;
        }
    }

    return dir;
}


vfsFileEntry*
vfsInsertFile( const char *fpath, void *addr, size_t size, uint32_t flags )
{
    auto lock = kthread::yield_guard(kthread::lock_count);
    VFS_SYSLOG("vfsInsertFile");
    VFS_LOG("fpath:  \"%s\"", fpath);

    fs::filepath path(fpath);
    vfsDirEntry *dir = vfsFindDirectory(path.m_dirname.c_str());

    if (!dir)
    {
        dir = vfsInsertDirectory(path.m_dirname.c_str());
    }

    vfsEntry *child = dir->getChild(path.m_filename.c_str());

    if (!child)
    {
        auto *file = dir->giveChild<vfsFileEntry>(path.m_filename.c_str(), flags, addr, size);
        VFS_LOG("created file \"%s\"", file->get_path());
        return file;
    }

    VFS_LOG("cannot create file, directory exists here");

    return nullptr;
}




vfsFileEntry*
vfsFindFile( vfsDirEntry *cwd, const char *fpath )
{
    kthread::yield_guard lock(kthread::lock_count);
    VFS_SYSLOG("vfsFindFile(%s)", fpath);

    if (fpath[0] == '/')
        cwd = vfsEntry::rootdir;

    fs::filepath path(fpath);
    auto &dirname  = path.m_dirname;
    auto &filename = path.m_filename;
    VFS_LOG("dirname, filename: \"%s\", \"%s\"", dirname.c_str(), filename.c_str());

    vfsDirEntry  *dir = vfsFindDirectory(cwd, dirname.c_str());
    vfsFileEntry *fh  = dir->getChild<vfsFileEntry>(filename.c_str());

    if (fh && fh->is_file())
    {
        VFS_LOG("found file: \"%s\"", fh->get_path());
        return fh;
    }

    VFS_LOG("no such directory \"%s%s\"", dir->get_path(), filename.c_str());
    return nullptr;
}


vfsFileEntry*
vfsFindFile( const char *fname )
{
    kthread::yield_guard lock(kthread::lock_count);
    VFS_SYSLOG("vfsFindFile(%s)", fname);
    return vfsFindFile(vfsEntry::rootdir, fname);
}

