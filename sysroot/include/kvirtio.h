#pragma once
#include <AAA.h>

#include <kdef.h>


// Ports
// ---------------------------------------------------------------------------------------------
// file int  sysc misc
// 0000 0000 0000 0000
// RWAE
#define KFILE 0xF000
#define KIRPT 0x0F00
#define KSYSC 0x00F0
#define KRESV 0x000F

#define KPORT_FILE KFILE
#define KPORT_IRPT KIRPT
#define KPORT_SYSC KSYSC
#define KPORT_RESV KRESV
// ---------------------------------------------------------------------------------------------


// Commands
// ---------------------------------------------------------------------------------------------
#define KREAD 0x8888
#define KWRTE 0x4444
#define KAPND 0x2222
#define KEXEC 0x1111


// ---------------------------------------------------------------------------------------------

// Targets
// ---------------------------------------------------------------------------------------------
#define KTRGT_STDERR 0b1000
#define KTRGT_STDIN  0b0100
#define KTRGT_STDOUT 0b0010
// ---------------------------------------------------------------------------------------------



// Ports but better
// ---------------------------------------------------------------------------------------------
#define KFILE_READ (KFILE | KREAD)
#define KFILE_WRTE (KFILE | KWRTE)
#define KFILE_APND (KFILE | KAPND)
#define KFILE_EXEC (KFILE | KEXEC)

// ---------------------------------------------------------------------------------------------


void     outvirt8( uint16_t port, uint8_t );
uint8_t  invirt8( uint16_t port );

void     outvirt16( uint16_t port, uint16_t );
uint16_t invirt16( uint16_t port );

void     outvirt32( uint16_t port, uint32_t );
uint32_t invirt32( uint16_t port );

void     outvirt64( uint16_t port, uint64_t );
uint64_t invirt64( uint16_t port );



typedef struct
{
    uint16_t  base;
    uint16_t  end;
    uint8_t  *ports;
    void (*recv)(uint16_t, uint64_t, size_t);
    void (*send)(uint16_t, uint64_t, size_t);
} virtiodev;


virtiodev *virtio_map( uint16_t, uint16_t, void (*)(uint16_t, uint64_t, size_t) );
bool virtio_unmap( virtiodev* );



#include <ZZZ.h>
