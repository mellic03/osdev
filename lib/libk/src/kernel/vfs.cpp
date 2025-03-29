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

    syslog log("vfsInsertDirectory( const char* )");
    log("root: 0x%lx", m_root);
    log("num_dirs: %lu", num_dirs);
    log("path: \"%s\"", path);

    vfsDirEntry *dir = m_root;

    while (count < num_dirs)
    {
        end = seek_brk(start, "/\0");
        strncpy(name, start, end-start+1);
        log("name: \"%s\"", name);
        // log("start: \'%c\' 0x%lx", *start, start);
        // log("end:   \'%c\' 0x%lx", *end, end);
        start = end+1;
        auto *child = dir->getChild(name);

        if (!child)
        {
            log("%s.giveChild<vfsDirEntry>(%s)", dir->name, name);
            child = dir->giveChild<vfsDirEntry>(name);
            log("child = 0x%lx", child);
        }
    
        else if (child->is_file())
        {
            return nullptr;
        }

        dir = static_cast<vfsDirEntry*>(child);

        count++;
    }

    log("created directory: \"%s\"", dir->get_path());

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

    syslog log("vfsFindDirectory( vfsDirEntry*, const char* )");
    log("cwd: \"%s\"", cwd->get_path());
    log("num_dirs: %lu", num_dirs);
    log("path: \"%s\"", path);

    vfsDirEntry *dir = cwd;

    while (count < num_dirs)
    {
        end = seek_brk(start, "/\0");
        strncpy(name, start, end-start+1);
        start = end+1;

        log("name: \"%s\"", name);

        auto *child = dir->getChild(name);

        if (!child || child->is_file())
        {
            log("could not vfsFind directory");
            return nullptr;
        }

        dir = static_cast<vfsDirEntry*>(child);

        count++;
    }

    log("found directory: \"%s\"", dir->get_path());

    return dir;
}


vfsFileEntry*
kfilesystem::vfsInsertFile( const char *dr, const char *fname,
                            void *addr, size_t size, uint8_t type )
{
    syslog log("vfsInsertFile( const char*, const char*, uintptr_t, size_t )");

    auto *dir = vfsFindDirectory(dr);

    if (!dir) dir = vfsInsertDirectory(dr);
    if (!dir) return nullptr;

    vfsEntry *entry = dir->getChild(fname);

    if (entry)
    {
        if (entry->is_dir())
        {
            log("path already exists as directory");
            return nullptr;
        }
    
        log("file already exists");
        return static_cast<vfsFileEntry*>(entry);
    }

    auto *file = dir->giveChild<vfsFileEntry>(
        fname, type, addr, size
    );

    // file->type = type;
    // file->addr = (void*)addr;
    // file->size = size;
    log("created file \"%s\"", file->get_path());

    return file;
}



vfsFileEntry*
kfilesystem::vfsFindFile( const char *fname )
{
    return vfsFindFile(m_root, fname);
}



static klock_t trie_lock = { false };

vfsFileEntry*
kfilesystem::vfsFindFile( vfsDirEntry *cwd, const char *pth )
{
    klock_acquire(&trie_lock);

    syslog log("vfsFindFile");

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
        log("name: \"%s\"", name);
    
        auto *child = dir->getChild(name);

        if (child && child->is_dir())
            dir = static_cast<vfsDirEntry*>(child);
        else
        {
            klock_release(&trie_lock);
            return nullptr;
        }

        count++;
    }

    end = seek_brk(start, "/\0");
    strncpy(name, start, end-start);
    name[end-start] = '\0';
    log("name: \"%s\"", name);

    auto *child = dir->getChild(name);

    if (child && child->is_file())
    {
        klock_release(&trie_lock);
        return static_cast<vfsFileEntry*>(child);
    }
    else
    {
        klock_release(&trie_lock);
        return nullptr;
    }

}

