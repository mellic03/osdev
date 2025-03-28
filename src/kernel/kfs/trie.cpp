#include "trie.hpp"
#include "../log/log.hpp"
#include <stdio.h>
#include <string.h>
#include <kstring.h>
#include <kmalloc.h>



KFS::Trie::Trie()
:   m_root(new vfsDirEntry(nullptr, "root"))
{
    memset(m_buf0, 0, sizeof(m_buf0));
    memset(m_buf1, 0, sizeof(m_buf1));
}




// kfsEntry*
// KFS::Trie::find( const char *path )
// {
//     // if (m_root == nullptr)
//     // {
//     //     m_root = new vfsDirEntry(nullptr, "root");
//     // }

//     // vfsDirEntry *dir = m_root;
//     // static char name[64];
//     // memset(name, 0, sizeof(name));

//     // while (*path && dir)
//     // {
//     //     auto *A = skip_ch(path, '/');
//     //     auto *B = seek_ch(A+1, '/');
//     //     memcpy(name, A, B-A);
//     //     name[B-A] = '\0';

//     //     auto *child = dir->getChild(name);
//     //     if (!child)
//     //     {
//     //         return nullptr;
//     //     }

//     //     dir = dynamic_cast<vfsDirEntry*>(child);

//     //     path = B;
//     // }

//     return nullptr;
// }




vfsDirEntry*
KFS::Trie::insertDirectory( const char *pth )
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

    syslog log("KFS::Trie::insertDirectory( const char* )");
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
KFS::Trie::findDirectory( const char *pth )
{
    return findDirectory(m_root, pth);
}


vfsDirEntry*
KFS::Trie::findDirectory( vfsDirEntry *cwd, const char *pth )
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

    syslog log("KFS::Trie::findDirectory( vfsDirEntry*, const char* )");
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
            log("could not find directory");
            return nullptr;
        }

        dir = static_cast<vfsDirEntry*>(child);

        count++;
    }

    log("found directory: \"%s\"", dir->get_path());

    return dir;
}


vfsFileEntry*
KFS::Trie::insertFile( const char *dr, const char *fname,
                       void *addr, size_t size, uint8_t type )
{
    syslog log("KFS::Trie::insertFile( const char*, const char*, uintptr_t, size_t )");

    auto *dir = findDirectory(dr);

    if (!dir) dir = insertDirectory(dr);
    if (!dir) return nullptr;

    kfsEntry *entry = dir->getChild(fname);

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

    vfsFileEntry *file = dir->giveChild<vfsFileEntry>(fname);

    file->type = type;
    file->addr = (void*)addr;
    file->size = size;
    log("created file \"%s\"", file->get_path());

    return file;
}


vfsFileEntry*
KFS::Trie::insertFile( const char *dr, const char *fname, kfstream *stream )
{
    syslog log("KFS::Trie::insertFile( const char*, const char*, kfstream* )");

    vfsFileEntry *file = insertFile(
        dr, fname, (void*)stream, stream->m_size, vfsFileType_STREAM
    );

    return file;
}




vfsFileEntry*
KFS::Trie::findFile( const char *fname )
{
    return findFile(m_root, fname);
}



static klock_t trie_lock = { false };

vfsFileEntry*
KFS::Trie::findFile( vfsDirEntry *cwd, const char *pth )
{
    klock_acquire(&trie_lock);

    syslog log("KFS::Trie::findFile");

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

