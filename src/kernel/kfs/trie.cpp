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

    auto  len   = strlen(path);
    char *name  = m_buf1;
    const char *start = path;
    const char *end   = start;

    syslog log("KFS::Trie::insertDirectory");
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
            child = dir->giveChild<vfsDirEntry>(name);
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

    syslog log("KFS::Trie::findDirectory");
    log("cwd: \"%s\"", cwd->get_path());
    log("num_dirs: %lu", num_dirs);
    log("path: \"%s\"", path);

    vfsDirEntry *dir = cwd;

    while (count < num_dirs)
    {
        end = seek_brk(start, "/\0");
        strncpy(name, start, end-start+1);
        start = end+1;

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
KFS::Trie::insertFile( const char *dr, const char *fname, uintptr_t addr, size_t size )
{
    syslog log("KFS::Trie::insertFile");

    vfsDirEntry *dir = findDirectory(dr);

    if (!dir)
    {
        dir = insertDirectory(dr);
    }

    if (!dir)
    {
        return nullptr;
    }

    log("dir: %s", dir->name);

    if (dir->hasChild(fname))
    {
        log("file already exists");
        return nullptr;
    }

    auto *file  = dir->giveChild<vfsFileEntry>(fname);
    file->base  = (uint8_t*)addr;
    file->top   = file->base;
    file->read  = &file->base;
    file->write = &file->base;
    file->eof   = file->base + size;
    file->size  = size;

    return static_cast<vfsFileEntry*>(file);
}


vfsFileEntry*
KFS::Trie::findFile( const char *fname )
{
    return findFile(m_root, fname);
}


vfsFileEntry*
KFS::Trie::findFile( vfsDirEntry *cwd, const char *pth )
{
    syslog log("KFS::Trie::findFile");
    log("cwd:  \"%s\"", cwd->get_path());
    log("path: \"%s\"", pth);

    size_t plen = strlen(pth);
    char  *path = m_buf0;

    if (pth[plen-1] != '/')
        sprintf(path, "%s/", pth);
    else
        sprintf(path, "%s", pth);


    size_t num_dirs = count_ch(pth, '/');
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
        log("name: \"%s\"", name);
    
        auto *child = dir->getChild(name);

        if (child && child->is_directory())
            dir = static_cast<vfsDirEntry*>(child);
        else
            return nullptr;

        count++;
    }

    end = seek_brk(start, "/\0");
    strncpy(name, start, end-start);
    name[end-start] = '\0';
    log("name: \"%s\"", name);

    auto *child = dir->getChild(name);

    if (child && child->is_file())
        return static_cast<vfsFileEntry*>(child);
    else
        return nullptr;

}

