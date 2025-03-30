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
vfsDirEntry *vfsEntry::rootdir = new vfsDirEntry(vfsEntry::rootdir, "R:");
// static vfsDirEntry *m_root = new vfsDirEntry(m_root, "R");
static std::mutex vfs_mutex;


vfsDirEntry*
kfilesystem::vfsInsertDirectory( const char *dpath )
{
    kthread::refc();

    // syslog log("vfsInsertDirectory");
    // log("dpath:  \"%s\"", dpath);

    vfsDirEntry *dir = vfsEntry::rootdir;
    fs::directorypath path(dpath);

    for (auto &name: path.m_sep)
    {
        auto *child = dir->getChild<vfsEntry>(name);

        if (!child)
        {
            child = dir->giveChild<vfsDirEntry>(name);
        }

        else if (child->is_file())
        {
            // log("Cannot create directory \"%s\". File with same name exists");
            return nullptr;
        }

        // log("\"%s\" --> \"%s\"", dir->name, name.c_str());
        dir = static_cast<vfsDirEntry*>(child);
    }

    // log("created directory \"%s\"", dir->get_path());

    return dir;
}


vfsDirEntry*
kfilesystem::vfsFindDirectory( const char *pth )
{
    return vfsFindDirectory(vfsEntry::rootdir, pth);
}


vfsDirEntry*
kfilesystem::vfsFindDirectory( vfsDirEntry *cwd, const char *dpath )
{
    kthread::refc();
    syslog log("vfsFindDirectory(%s)", dpath);

    if ((strlen(dpath) == 1) && (dpath[0] == '/'))
        return vfsEntry::rootdir;
    if (dpath[0] == '/')
        cwd = vfsEntry::rootdir;

    fs::directorypath path(dpath);
    auto dirname = path.m_dirname;
    log("dirname: %s", dirname.c_str());

    vfsDirEntry *dir = cwd;

    for (auto &name: path.m_sep)
    {
        log("name: %s", name.c_str());
        
        if (name == ".")
        {
            continue; // dir =  dir;
        }

        else if (name == "..")
        {
            dir = (dir->parent) ? dir->parent : dir;
            continue;
        }

        vfsEntry *child = dir->getChild(name);

        if (!child)
        {
            log("no such directory \"%s%s\"", dir->get_path(), name.c_str());
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
kfilesystem::vfsInsertFile( const char *fpath, void *addr, size_t size, uint32_t flags )
{
    kthread::refc();
    // syslog log("vfsInsertFile");
    // log("fpath:  \"%s\"", fpath);

    fs::filepath path(fpath);
    vfsDirEntry *dir = vfsFindDirectory(path.m_dirname.c_str());

    if (!dir)
    {
        dir = vfsInsertDirectory(path.m_dirname.c_str());
    }

    vfsEntry *child = dir->getChild(path.m_filename);

    if (!child)
    {
        auto *file = dir->giveChild<vfsFileEntry>(path.m_filename, flags, addr, size);
        // log("created file \"%s\"", file->get_path());
        return file;
    }

    // log("file or directory already exists");
    return nullptr;
}




vfsFileEntry*
kfilesystem::vfsFindFile( vfsDirEntry *cwd, const char *fpath )
{
    kthread::refc();
    syslog log("vfsFindFile(%s)", fpath);

    if (fpath[0] == '/')
        cwd = vfsEntry::rootdir;

    fs::filepath path(fpath);
    auto &dirname  = path.m_dirname;
    auto &filename = path.m_filename;
    log("dirname, filename: \"%s\", \"%s\"", dirname.c_str(), filename.c_str());

    vfsDirEntry  *dir = vfsFindDirectory(cwd, dirname.c_str());
    vfsFileEntry *fh  = dir->getChild<vfsFileEntry>(filename);

    if (fh && fh->is_file())
    {
        log("found file: \"%s\"", fh->get_path());
        return fh;
    }

    log("no such directory \"%s%s\"", dir->get_path(), filename.c_str());
    return nullptr;
}


vfsFileEntry*
kfilesystem::vfsFindFile( const char *fname )
{
    return vfsFindFile(vfsEntry::rootdir, fname);
}

