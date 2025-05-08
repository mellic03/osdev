// #pragma once
// #include <kdef.h>
// #include <stddef.h>
// #include <mutex>



// enum ipcprt_status: uint8_t
// {
//     PORT_CLOSED   = 1,
//     PORT_OPEN     = 2,
//     PORT_REFUSED  = 3,
//     PORT_ACCEPTED = 4,
// };



// ipcprt_status ipcport_open( uint16_t port );
// ipcprt_status ipcport_connect( uint16_t port, uint64_t timeout );
// size_t ipcport_send( uint16_t port, const void *src, size_t packets, size_t pktsize );
// size_t ipcport_recv ( uint16_t port, void *dst, size_t packets, size_t pktsize );

// // void     ipc_writen( uint16_t port, void *src, size_t n, size_t stride );
// // size_t   ipc_readn ( uint16_t port, void *buf, size_t n, size_t stride );

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










// struct RingBuffer;


// struct io_head
// {
//     RingBuffer &B;
//     io_head    &otr;
//     size_t      idx;
//     io_head( RingBuffer&, io_head& );
//     uint8_t *operator++(int);
// };



// struct RingBuffer
// {
// private:
//     using callback_t = void (*)();

// public:
//     uint8_t   *base;
//     uint8_t   *end;
//     io_head    read;
//     io_head    write;
//     size_t     m_cap;
//     size_t     m_size;
//     size_t     m_stride;
//     std::mutex m_mutex;
//     callback_t m_flsh;

//     RingBuffer(): read(*this, write), write(*this, read) {  };
//     RingBuffer( size_t cap, size_t stride, void (*fn)() = nullptr );

//     // size_t read_n( void *dst, size_t n );
//     // size_t write_n( const void *src, size_t n );
//     // void   flush();
// };



// // void   RingBuffer_init( RingBuffer*, size_t nbytes, size_t stride );
// // size_t RingBuffer_recv( RingBuffer*, void *dst, size_t nbytes );
// // size_t RingBuffer_send( RingBuffer*, const void *src, size_t nbytes );
