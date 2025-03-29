#include <kernel/vfs.hpp>
#include <kernel/log.hpp>
#include <stdio.h>
#include <string.h>
#include <kstring.h>
#include <kmalloc.h>


static char m_buf0[128];
static char m_buf1[128];
static vfsDirEntry *m_root = new vfsDirEntry(nullptr, "root");


vfsDirEntry*
kfilesystem::vfsInsertDirectory( const char *pth )
{
    memset(m_buf0, 0, sizeof(m_buf0));
    memset(m_buf1, 0, sizeof(m_buf1));

    size_t plen = strlen(pth);
    char  *path = m_buf0;

    if (pth[plen-1] != '/')
        sprintf(path, "%s/", pth);
    else
        sprintf(path, "%s", pth);

    // home/tty0/
    size_t num_dirs = count_ch(path, '/');
    size_t count = 0;

    char *name  = m_buf1;
    const char *start = path;
    const char *end   = start;

    vfsDirEntry *dir = m_root;

    while (count < num_dirs)
    {
        end = seek_brk(start, "/\0");
        strncpy(name, start, end-start+1);
        start = end+1;
        auto *child = dir->getChild(name);

        if (!child)
        {
            child = dir->giveChild<vfsDirEntry>(name);
        }
    
        else if (child->is_file())
        {
            return nullptr;
        }

        dir = static_cast<vfsDirEntry*>(child);

        count++;
    }

    return dir;
}


vfsDirEntry*
kfilesystem::vfsFindDirectory( const char *pth )
{
    return vfsFindDirectory(m_root, pth);
}


vfsDirEntry*
kfilesystem::vfsFindDirectory( vfsDirEntry *cwd, const char *pth )
{
    if (strcmp(pth, "/") == 0)
    {
        return m_root;
    }

    if (pth[0] == '/')
    {
        cwd = m_root;
        pth++;
    }

    memset(m_buf0, 0, sizeof(m_buf0));
    memset(m_buf1, 0, sizeof(m_buf1));

    size_t plen = strlen(pth);
    char  *path = m_buf0;

    if (pth[plen-1] != '/')
        sprintf(path, "%s/", pth);
    else
        sprintf(path, "%s", pth);

    // home/tty0/
    size_t num_dirs = count_ch(path, '/');
    size_t count = 0;

    auto  len   = strlen(path);
    char *name  = m_buf1;
    const char *start = path;
    const char *end   = start;
    vfsDirEntry *dir = cwd;

    while (count < num_dirs)
    {
        end = seek_brk(start, "/\0");
        strncpy(name, start, end-start+1);
        start = end+1;

        auto *child = dir->getChild(name);

        if (!child || child->is_file())
        {
            return nullptr;
        }

        dir = static_cast<vfsDirEntry*>(child);

        count++;
    }

    return dir;
}


vfsFileEntry*
kfilesystem::vfsInsertFile( const char *dr, const char *fname,
                            void *addr, size_t size, uint8_t type )
{
    auto *dir = vfsFindDirectory(dr);

    if (!dir) dir = vfsInsertDirectory(dr);
    if (!dir) return nullptr;

    vfsEntry *entry = dir->getChild(fname);

    if (entry)
    {
        if (entry->is_dir())
        {
            return nullptr;
        }
    
        return static_cast<vfsFileEntry*>(entry);
    }

    auto *file = dir->giveChild<vfsFileEntry>(
        fname, type, addr, size
    );

    // file->type = type;
    // file->addr = (void*)addr;
    // file->size = size;

    return file;
}



vfsFileEntry*
kfilesystem::vfsFindFile( const char *fname )
{
    return vfsFindFile(m_root, fname);
}



vfsFileEntry*
kfilesystem::vfsFindFile( vfsDirEntry *cwd, const char *pth )
{
    kthread::global_lock();

    if (pth[0] == '/')
    {
        cwd = m_root;
        pth++;
    }

    size_t plen = strlen(pth);
    char  *path = m_buf0;
    
    // if (pth[plen-1] != '/')
    //     sprintf(path, "%s/", pth);
    // else
        sprintf(path, "%s", pth);


    size_t num_dirs = count_ch(pth, '/');
    size_t count = 0;

    char *name  = m_buf1;
    const char *start = path;
    const char *end   = start;

    vfsDirEntry *dir = cwd;

    while (count < num_dirs)
    {
        end = seek_brk(start, "/\0");
        strncpy(name, start, end-start+1);
        start = end+1;
    
        auto *child = dir->getChild(name);

        if (child && child->is_dir())
            dir = static_cast<vfsDirEntry*>(child);
        else
        {
            kthread::global_unlock();
            return nullptr;
        }

        count++;
    }

    end = seek_brk(start, "/\0");
    strncpy(name, start, end-start);
    name[end-start] = '\0';
    auto *child = dir->getChild(name);

    if (child && child->is_file())
    {
        kthread::global_unlock();
        return static_cast<vfsFileEntry*>(child);
    }
    else
    {
        kthread::global_unlock();
        return nullptr;
    }

}

