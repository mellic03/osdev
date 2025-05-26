#pragma once
#include <stddef.h>
#include <stdint.h>
#include <arch/io.hpp>


// Ports
#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC

// Config Address 
static constexpr auto aaa = 0x10;
static constexpr auto eee = 1<<4;

// Offset
#define PCI_VENDOR_ID            0x00
#define PCI_DEVICE_ID            0x02
#define PCI_COMMAND              0x04
#define PCI_STATUS               0x06
#define PCI_REVISION_ID          0x08
#define PCI_PROG_IF              0x09
#define PCI_SUBCLASS             0x0A
#define PCI_CLASS                0x0B
#define PCI_CACHE_LINE_SIZE      0x0C
#define PCI_LATENCY_TIMER        0x0D
#define PCI_HEADER_TYPE          0x0E
#define PCI_BIST                 0x0F
#define PCI_BAR0                 0x10
#define PCI_BAR1                 0x14
#define PCI_BAR2                 0x18
#define PCI_BAR3                 0x1C
#define PCI_BAR4                 0x20
#define PCI_BAR5                 0x24
#define PCI_INTERRUPT_LINE       0x3C
#define PCI_SECONDARY_BUS        0x09

// Device type
#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2
#define PCI_TYPE_BRIDGE         0x0604
#define PCI_TYPE_SATA           0x0106
#define PCI_NONE                0xFFFF



// struct pci_device
// {
//     uint32_t segment;
//     uint32_t segment_memory;
//     uint32_t bus;
//     uint32_t dev;
//     uint32_t fun;

//     uint32_t vendor_id;
//     uint32_t device_id;
//     uint32_t interrupt_pin;
//     uint32_t interrupt_line;
//     uint32_t msi_register;
//     uint32_t msi_x_register;

//     uint32_t bios_handoff_timeout;
// };
#define PCI_BAR_IO                      0x01
#define PCI_BAR_LOWMEM                  0x02
#define PCI_BAR_64                      0x04
#define PCI_BAR_PREFETCH                0x08

static constexpr uint64_t BIT_IO       = 0b0001;
static constexpr uint64_t BIT_64       = 0x04;
static constexpr uint64_t BIT_PREFETCH = 0b0100;


struct bar_memspace_t
{
    uint32_t is_iospace:    1;
    uint32_t type:          2;
    uint32_t prefetchable:  1;
    uint32_t address:       28;
} __attribute__((packed));

struct bar_iospace_t
{
    uint32_t is_iospace:    1;
    uint32_t reserved:      1;
    uint32_t address:      30;
} __attribute__((packed));


union PCIBarInfo
{
    uint32_t is_iospace: 1;
    bar_memspace_t memspace;
    bar_iospace_t  iospace;
};

struct PCI_Device
{
    uint16_t vendor_id; // PCI Vendor ID
    uint16_t device_id; // PCI Device ID

    uint8_t bus;  // PCI Bus
    uint8_t slot; // PCI Slot
    uint8_t func; // PCI Func

    uint8_t baseclass; // PCI Class
    uint8_t subclass;  // PCI Subclass
    uint8_t progIf;    // PCI Programming Interface

    uintptr_t bars[6]; // PCI BARs

    PCI_Device();
    PCI_Device( uint16_t bs, uint16_t st, uint16_t fn );
};


// struct PCI_dev
// {
//     PCI_dev() {  };
//     PCI_dev( uint32_t bus, uint32_t dev, uint32_t fun );

//     uint16_t vendor_id;
//     uint16_t device_id;
//     uint32_t bar[6];

//     union {
//         uint32_t bits;
//         struct {
//             uint32_t always_zero  : 2;
//             uint32_t field_num    : 6;
//             uint32_t function_num : 3;
//             uint32_t device_num   : 5;
//             uint32_t bus_num      : 8;
//             uint32_t reserved     : 7;
//             uint32_t enable       : 1;
//         } field;
//     };

//     uint16_t read16( uint32_t off )
//     {
//         if((off & 0x03) == 0x3)
//             return 0;
//         IO::out32(0xcf8, (1 << 31) | (field.bus_num << 16) | (field.device_num << 11) | (field.function_num << 8) | (off & 0xfc));
//         return IO::in16(0xcfc + (off & 0x03));
//     }

//     void write16( uint32_t off, uint16_t value )
//     {
//         field.field_num = (off & 0xFC) >> 2;
//         field.enable = 1;
//         IO::out32(PCI_CONFIG_ADDRESS, this->bits); // Where to write
//         IO::out32(PCI_CONFIG_DATA, value);         // What to write
//     }

// };


namespace PCI
{
    void init();
    PCI_Device findDevice( uint16_t vendor_id, uint16_t device_id );
    

    // Bridge type
    static constexpr uint32_t CLASS_BRIDGE           = 0x06;
    static constexpr uint32_t BRIDGE_HOST            = 0x00;
    static constexpr uint32_t BRIDGE_ISA             = 0x01;
    static constexpr uint32_t BRIDGE_EISA            = 0x02;
    static constexpr uint32_t BRIDGE_MCA             = 0x03;
    static constexpr uint32_t BRIDGE_PCI             = 0x04;
    static constexpr uint32_t BRIDGE_PCMCIA          = 0x05;
    static constexpr uint32_t BRIDGE_NUBUS           = 0x06;
    static constexpr uint32_t BRIDGE_CARDBUS         = 0x07;
    static constexpr uint32_t BRIDGE_RACEWAY         = 0x08;
    static constexpr uint32_t BRIDGE_OTHER           = 0x80;
 
    

    static constexpr uint32_t CLASS_STORAGE     = 0x01;
    static constexpr uint32_t STORAGE_SCSI      = 0x00;
    static constexpr uint32_t STORAGE_IDE       = 0x01;
    static constexpr uint32_t STORAGE_FLOPPY    = 0x02;
    static constexpr uint32_t STORAGE_IPI       = 0x03;
    static constexpr uint32_t STORAGE_RAID      = 0x04;
    static constexpr uint32_t STORAGE_SATA      = 0x06;
    static constexpr uint32_t STORAGE_SAS       = 0x07;
    static constexpr uint32_t STORAGE_OTHER     = 0x80;

}
