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
        strncpy(name, start, end-start);
        // log("name: \"%s\"", name);
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
    log("root: 0x%lx", m_root);
    log("num_dirs: %lu", num_dirs);
    log("path: \"%s\"", path);

    vfsDirEntry *dir = m_root;

    while (count < num_dirs)
    {
        end = seek_brk(start, "/\0");
        strncpy(name, start, end-start);
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
KFS::Trie::insertFile( const char *pth )
{

    return nullptr;
}


vfsFileEntry*
KFS::Trie::findFile( const char *pth )
{

    return nullptr;
}


