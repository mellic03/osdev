#include <kfstream.hpp>
#include <kmalloc.h>
#include <string.h>



kfstream::kfstream()
:   m_base(0), m_eof(0), m_size(0),
    m_read(0), m_write(0),
    m_lock({false}), m_flush(nullptr)
{

}



kfstream::kfstream( size_t nbytes, void (*fsh)(kfstream*) )
{
    m_base  = (uint8_t*)kmalloc(nbytes);
    m_eof   = m_base + nbytes;
    m_size  = nbytes;
    m_read  = m_base;
    m_write = m_base;
    m_lock  = { false };
    m_flush = fsh;
}


size_t
kfstream::write( const void *src, size_t nbytes )
{
    klock_acquire(&m_lock);

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
    klock_release(&m_lock);

    return count;
}


size_t
kfstream::read( void *dst, size_t nbytes )
{
    klock_acquire(&m_lock);

    uint8_t *d = (uint8_t*)dst;
    size_t count = 0;

    while ((count < nbytes) && (m_read < m_write))
    {
        *(d++) = (*m_read++);
        count += 1;
    }

    klock_release(&m_lock);

    return count;
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

