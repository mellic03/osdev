#include <filesystem/vfs2.hpp>
#include <kernel/kstring.h>
#include <kernel/log.hpp>

#include <string.h>
#include <kassert.h>
#include <kmalloc.h>

vfs2Entry *vfs2::root;


vfs2Header::vfs2Header( const char *nm, uint64_t tp, size_t sz, void *data )
:   type(tp), size(sz)
{
    memset(name, '\0', sizeof(name));
    strncpy(name, nm, sizeof(name));

    uint8_t *base = (uint8_t*)((data) ? data : kmalloc(sz));

    sof = base;
    eof = base + sz;
    pos = sof;
};




size_t vfs2RawFile::write( const void *src, size_t n )
{
    auto *srcbuf = (const uint8_t*)src;
    auto *srcend = srcbuf + n;
    auto *srcpos = srcbuf;

    while ((srcpos < srcend) && (pos < this->eof))
    {
        *(pos++) = *(srcpos++);
    }

    return srcpos - srcbuf;
}

size_t vfs2RawFile::write_str( const char *src )
{
    return write(src, strlen(src));
}








template <typename T>
static vfs2Entry *createEntry( const char *name, uint64_t size, void *data )
{
    size_t obj_size = sizeof(T);

    // data is embedded at end of struct
    if (data == nullptr)
    {
        obj_size += size;
    }

    T *entry = (T*)kmalloc(obj_size);
       entry = new (entry) T(name, obj_size, data);
    // syslog::printf("[createEntry] \"%s\" at 0x%lx\n", name, entry->sof);

    return (vfs2Entry*)entry;
}



void vfs2::init()
{
    syslog log("vfs2::init");
    // log("base: 0x%lx", base);
    // log("size: %lu KB", size/1024);
    vfs2::root = createEntry<vfs2DirEntry>("root", 64*sizeof(vfs2Entry*), nullptr);
}





static vfs2Entry *findChild( vfs2DirEntry *dir, const char *name )
{
    kassert((dir->type == vfs2Entry_Directory));

    for (uint32_t i=0; i<dir->count; i++)
    {
        vfs2Entry *entry = dir->entries[i];

        if (entry && strcmp(entry->name, name) == 0)
        {
            return entry;
        }
    }

    return nullptr;
}



vfs2Entry*
vfs2::resize( vfs2Entry *fh, size_t nbytes )
{
    if (fh->size || nbytes)
    {

    }

    return nullptr;
}





vfs2Entry*
vfs2::find( const char *path )
{
    auto *A = skip_ch(path, ' ');
          A = skip_ch(A,    '/');
    auto *B = seek_ch(A,    '/');

    static char name[64];
    memset(name, '\0', sizeof(name));

    vfs2Entry *curr = vfs2::root;

    while (A < B)
    {
        strncpy(name, A, B-A);

        if (curr->type != vfs2Entry_Directory)
        {
            return nullptr;
        }

        vfs2DirEntry *dir  = (vfs2DirEntry*)curr;
        vfs2Entry    *next = findChild(dir, name);

        if (!next)
        {
            return nullptr;
        }

        if (next && (*B == '\0'))
        {
            return next;
        }

        curr = next;
        A = skip_ch(B, '/');
        B = seek_ch(A, '/');
    }

    return nullptr;
}


vfs2Entry*
vfs2::open( const char *path, uint64_t nbytes, void *data )
{
    auto *A = skip_ch(path, ' ');
          A = skip_ch(A,    '/');
    auto *B = seek_ch(A,    '/');

    static char name[64];
    memset(name, '\0', sizeof(name));

    vfs2Entry *curr = vfs2::root;

    while (A < B)
    {
        strncpy(name, A, B-A);
        // log("-----------------");
        // log("A:     %lu", A - path);
        // log("B:     %lu", B - path);
        // log("name: \"%s\"", name);
        // log("curr->type: %lu", curr->type);

        if (curr->type != vfs2Entry_Directory)
        {
            return nullptr;
        }

        vfs2DirEntry *dir  = (vfs2DirEntry*)curr;
        vfs2Entry    *next = findChild(dir, name);
        // log("next == 0x%lx", next);
    
        if (next)
        {
            if (*B == '\0')
            {
                return next;
            }
        }

        else if (*B == '\0')
        {
            // log("*B == \\0");
            next = createEntry<vfs2Entry>(name, nbytes, data);
            dir->entries[dir->count++] = next;
            return next;
        }

        else if (next == nullptr)
        {
            next = createEntry<vfs2DirEntry>(name, 32*sizeof(vfs2Entry*), nullptr);
            dir->entries[dir->count++] = next;
        }

        curr = next;
        A = skip_ch(B, '/');
        B = seek_ch(A, '/');
    }

    kassert(false); // Should be unreachable

    return nullptr;
}




void vfs2::walk( vfs2Entry *entry )
{
    syslog::pushIndent(2);

    if (entry->type != vfs2Entry_Directory)
    {
        syslog::print("- %s\n", entry->name);
        syslog::popIndent(2);
        return;
    }

    syslog::print("%s/\n", entry->name);

    auto *dir = (vfs2DirEntry*)entry;
    for (uint32_t i=0; i<dir->count; i++)
    {
        auto &E = dir->entries[i];

        if (E)
        {
            vfs2::walk(E);
        }
    }

    syslog::popIndent(2);
}


