// #include <kfstream.hpp>
// #include <string.h>



// kfstream::kfstream()
// :   m_base(0), m_eof(0), m_size(0),
//     m_read(0), m_write(0),
//     m_flush(nullptr)
// {

// }


// kfstream::kfstream( size_t size, void (*flsh)(kfstream*) )
// :   kfstream(new uint8_t[size], size, flsh)
// {

// }


// kfstream::kfstream( void *base, size_t size, void (*flsh)(kfstream*) )
// {
//     m_base  = (uint8_t*)base;
//     m_eof   = m_base + size;
//     m_size  = size;
//     m_read  = 0;
//     m_write = 0;
//     m_flush = flsh;
// }


// size_t
// kfstream::read( void *dstptr, size_t nbytes )
// {
//     // m_mutex.lock();

//     auto *dst = static_cast<uint8_t*>(dstptr);
//     size_t count = 0;

//     while (count < nbytes && m_read != m_write)
//     {
//         *dst++ = m_base[m_read];
//         m_read = (m_read + 1) % m_size;
//         count++;
//     }

//     // m_mutex.unlock();
//     return count;
// }


// size_t
// kfstream::write( const void *srcptr, size_t nbytes )
// {
//     // m_mutex.lock();

//     auto *src = static_cast<const uint8_t*>(srcptr);
//     size_t count = 0;

//     while (count < nbytes)
//     {
//         size_t next_write = (m_write + 1) % m_size;

//         if (next_write == m_read)
//         {
//             flush();
//             break;
//         }

//         m_base[m_write] = *src++;
//         m_write = next_write;
//         count++;
//     }

//     // m_mutex.unlock();
//     return count;
// }


// // size_t
// // kfstream::readstr( char *dst )
// // {
// //     uint8_t *start = m_read;

// //     while (*m_read && (m_read < m_write))
// //     {
// //         *(dst++) = *(m_read++);
// //     }
// //     kthread::global_unlock();

// //     return m_read - start;
// // }

// // size_t
// // kfstream::writestr( const char *src )
// // {
// //     auto *start = src;

// //     while (*src && (m_write < m_eof))
// //     {
// //         *(m_write++) = *(src++);
// //     }

// //     kthread::global_unlock();

// //     return src - start;
// // }



// void
// kfstream::flush()
// {
//     if (m_flush)
//     {
//         m_flush(this);
//     }

//     else
//     {
//         m_read  = 0;
//         m_write = 0;
//     }
// }

