#pragma once
#include <AAA.h>
#include <kthread.hpp>



struct kfstream
{
private:
    using fshtype = void (*)(kfstream*);
public:

    uint8_t  *m_base;
    uint8_t  *m_eof;
    size_t    m_size;
    uint8_t  *m_read;
    uint8_t  *m_write;
    klock_t   m_lock;
    fshtype   m_flush;

    kfstream();
    kfstream( size_t sz, void (*flush)(kfstream*) = nullptr );

    size_t write( const void *src, size_t nbytes );
    size_t read( void *dst, size_t nbytes );
    void   flush();
};





#include <ZZZ.h>
