// #include "ipc.hpp"
// #include <stdio.h>
// #include <string.h>
// #include <kmalloc.h>
// #include <kthread.hpp>
// #include <kernel/log.hpp>
// #include <mutex>


// io_head::io_head( RingBuffer &buf, io_head &otr )
// :   B(buf), otr(otr), idx(0)
// {

// }

// uint8_t*
// io_head::operator++(int)
// {
//     size_t offset = idx*B.m_stride;
//     idx = (idx+1) % B.m_cap;
//     return B.base + offset;
// }



// RingBuffer::RingBuffer( size_t cap, size_t stride, void (*fn)() )
// :   base  ((uint8_t*)kmalloc(cap*stride)),
//     end   (base + cap*stride),
//     read  (*this, write),
//     write (*this, read),
//     m_cap   (cap),
//     m_size  (0),
//     m_flsh  (fn)
// {

    
// }






// // struct ipc_pctl
// // {
// //     uint16_t  base;
// //     uint16_t  end;
// //     uint8_t  *ports;

// //     void (*recv)(uint16_t, uint64_t, size_t);
// //     void (*send)(uint16_t, uint64_t, size_t);
// // };

// // bool ipc_mapctl( ipc_pctl* );
// // bool ipc_unmapctl( ipc_pctl* );



// #define MAX_PORT 0xFFFF
// #define BUFFER_SIZE 4096

// // template <typename T>
// // class IpcPort
// // {
// // private:
// //     size_t cap;
// //     T     *buf;
// //     T     *end;
// //     size_t rd;
// //     size_t wt;
// //     int    size;
// //     std::mutex lock;

// // public:

// //     IpcPort()
// //     :   cap (256),
// //         buf ((T*)kmalloc(cap * sizeof(T))),
// //         end (buf + cap),
// //         rd  (0),
// //         wt  (0)
// //     {

// //     }

// //     bool push( T value )
// //     {
// //         if (size >= cap)
// //             return false;

// //         buf[w++] = value;
// //         size++;
// //         return true;
// //     }

// //     bool pop( T *value )
// //     {
// //         if (size <= 0)
// //             return false;

// //         *value = buf[r++];
// //         size--;
// //         return true;
// //     }
// // };

// #include <vector>
// #include <tuple>

// // template <typename key_type, typename value_type>
// // class notgreat_map
// // {
// // private:
// //     using store_type = std::pair<key_type, value_type>;
// //     std::vector<store_type> lvl0;

// // public:
// //     notgreat_map()
// //     {

// //     }
// // };




// template <typename T>
// struct ipc_buffer
// {
//     int P;
//     int owner_pid;

//     T *buf;
//     T *end;
//     uint8_t  r=0;
//     uint8_t  w=0;
//     int      cap;
//     int      size;
//     size_t   stride;

//     ipc_buffer()
//     {
//         buf  = (T*)kmalloc(256);
//         end  = buf + 256;
//         r    = 0;
//         w    = 0;
//         cap  = 256;
//         size = 0;
//     }

//     bool push( T value )
//     {
//         if (size >= cap)
//             return false;

//         buf[w++] = value;
//         size++;
//         return true;
//     }

//     bool pop( T *value )
//     {
//         if (size <= 0)
//             return false;

//         *value = buf[r++];
//         size--;
//         return true;
//     }
// };




// struct ipc_port
// {
//     ipc_buffer<uint8_t>  ch8;
//     ipc_buffer<uint32_t> ch32;
//     ipc_buffer<uint64_t> ch64;

// private:
//     template <typename T>
//     bool push( ipc_buffer<T> &ch, const void *value )
//     {
//         return ch.push(*((T*)value));
//     }

//     template <typename T>
//     bool pop( ipc_buffer<T> &ch, void *value )
//     {
//         return ch.pop((T*)value);
//     }

// public:

//     bool push( const void *value, size_t stride )
//     {
//         switch (stride)
//         {
//             default: return false;
//             case 1: return push(ch8,  value);
//             case 4: return push(ch32, value);
//             case 8: return push(ch64, value);
//         }
//     }

//     bool pop( void *value, size_t stride )
//     {
//         switch (stride)
//         {
//             default: return false;
//             case 1: return pop(ch8,  value);
//             case 4: return pop(ch32, value);
//             case 8: return pop(ch64, value);
//         }
//     }

//     std::mutex m_lock;
//     void lock()   { m_lock.lock(); }
//     void unlock() { m_lock.unlock(); }
// };


// static int         test[4];
// static ipc_port   *ipc_ports[MAX_PORT];


// __attribute__((constructor))
// void ipcprt_init()
// {
//     test[0] = 0xD;
//     test[1] = 0xE;
//     test[2] = 0xA;
//     test[3] = 0xD;

//     for (int i=0; i<MAX_PORT; i++)
//     {
//         ipc_ports[i] = nullptr;
//     }
// }



// ipcprt_status
// ipcport_open( uint16_t P )
// {
//     if (P >= 0xFFFF)
//     {
//         return PORT_CLOSED;
//     }

//     if (ipc_ports[P] != nullptr)
//     {
//         return PORT_REFUSED;
//     }

//     ipc_ports[P] = new ipc_port;
//     return PORT_OPEN;
// }


// ipcprt_status
// ipcport_connect( uint16_t port, uint64_t timeout )
// {
//     for (uint64_t i=0; i<timeout; i++)
//     {
//         if (ipc_ports[port] != nullptr) // (ipc_ports[port].accept(port) == true)
//             return PORT_ACCEPTED;
//         asm volatile ("nop;");
//     }

//     if (port >= 0xFFFF)
//     {
//         return PORT_CLOSED;
//     }

//     if (ipc_ports[port] == nullptr)
//     {
//         return PORT_CLOSED;
//     }

//     if (1) // (ipc_ports[port].accept(port) == true)
//     {
//         return PORT_ACCEPTED;
//     }

//     return PORT_REFUSED;
// }



// size_t ipcport_send( uint16_t P, const void *src, size_t packets, size_t pktsize )
// {
//     if (ipc_ports[P] == nullptr)
//     {
//         return 0;
//     }

//     ipc_port &port = *ipc_ports[P];
//     port.lock();
//     auto  *srcbuf = (const uint8_t*)src;
//     size_t total = 0;

//     for (size_t i=0; i<packets; i++)
//     {
//         if (port.push(srcbuf + i*pktsize, pktsize) == false)
//         {
//             port.unlock();
//             return total;
//         }

//         total += 1;
//     }

//     port.unlock();

//     return total;
// }


// size_t ipcport_recv( uint16_t P, void *dst, size_t packets, size_t pktsize )
// {
//     if (ipc_ports[P] == nullptr)
//     {
//         return 0;
//     }

//     ipc_port &port = *ipc_ports[P];
//     port.lock();

//     uint8_t *dstbuf = (uint8_t*)dst;
//     size_t   total = 0;

//     for (size_t i=0; i<packets; i++)
//     {
//         if (port.pop(dstbuf + i*pktsize, pktsize) == false)
//         {
//             port.unlock();
//             return total;
//         }
//         total += 1;
//     }

//     port.unlock();
//     return total;
// }


