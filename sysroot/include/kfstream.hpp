#pragma once
#include <AAA.h>
#include <ZZZ.h>

#include <mutex>
#include <kthread.hpp>


struct kfstream
{
private:
    using flshtype = void (*)(kfstream*);
public:

    uint8_t   *m_base;
    uint8_t   *m_eof;
    size_t     m_size;
    size_t     m_read;
    size_t     m_write;
    std::mutex m_mutex;
    flshtype   m_flush;

    kfstream();
    kfstream( size_t size, void (*flsh)(kfstream*) = nullptr );
    kfstream( void *base, size_t sz, void (*flsh)(kfstream*) = nullptr );

    size_t read( void *dst, size_t nbytes );
    size_t write( const void *src, size_t nbytes );
    // size_t readstr( char *dst );
    // size_t writestr( const char *src );
    void   flush();
};


