#include <kfstream.hpp>
#include <string.h>



kfstream::kfstream()
:   m_base(0), m_eof(0), m_size(0),
    m_read(0), m_write(0),
    m_lock({false}), m_flush(nullptr)
{

}

kfstream::kfstream( size_t size, void (*flsh)(kfstream*) )
:   kfstream((void*)(new uint8_t[size]), size, flsh)
{

}

kfstream::kfstream( void *base, size_t size, void (*flsh)(kfstream*) )
{
    m_base  = (uint8_t*)base;
    m_eof   = m_base + size;
    m_size  = size;
    m_read  = m_base;
    m_write = m_base;
    m_lock  = { false };
    m_flush = flsh;
}


size_t
kfstream::read( void *dst, size_t nbytes )
{
    // kthread::global_lock();
    size_t count = 0;

    // if (m_read + nbytes < m_write)
    // {
    //     memcpy(dst, m_read, nbytes);
    //     m_read += nbytes;
    //     count = nbytes;
    // }

    // else
    // {
        uint8_t *d = (uint8_t*)dst;

        while ((count < nbytes) && (m_read < m_write))
        {
            *(d++) = *(m_read++);
            count += 1;
        }
    // }

    // kthread::global_unlock();

    return count;
}

size_t
kfstream::write( const void *src, size_t nbytes )
{
    // kthread::global_lock();

    const uint8_t *s = (const uint8_t*)src;
    size_t count = 0;

    while ((count < nbytes) && (m_write < m_eof))
    {
        *(m_write++) = *(s++);
        count += 1;
    }

    if (m_write >= m_eof)
    {
        flush();
    }
    // kthread::global_unlock();

    return count;
}


size_t
kfstream::readstr( char *dst )
{
    kthread::global_lock();
    uint8_t *start = m_read;

    while (*m_read && (m_read < m_write))
    {
        *(dst++) = *(m_read++);
    }
    kthread::global_unlock();

    return m_read - start;
}

size_t
kfstream::writestr( const char *src )
{
    kthread::global_lock();
    auto *start = src;

    while (*src && (m_write < m_eof))
    {
        *(m_write++) = *(src++);
    }

    kthread::global_unlock();

    return src - start;
}



void
kfstream::flush()
{
    if (m_flush)
    {
        m_flush(this);
    }

    else
    {
        m_read  = m_base;
        m_write = m_base;
    }
}

