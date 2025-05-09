#pragma once
#include <stddef.h>
#include <stdint.h>
#include <kernel/interrupt.hpp>

struct ksym_t;


// using DevMain_t  = void (*)(void*);
// using DevInit_t  = void (*)(ksym_t*);
// using DevClose_t = void (*)(void);
// using DevRead_t  = size_t (*)(void*, size_t);
// using DevWrite_t = size_t (*)(const void*, size_t);
// using DevIrq_t   = void(*)(intframe_t*);



const char *ModuleTypeStr( uint64_t type );
const char *DeviceTypeStr( uint64_t type );
const char *DaemonTypeStr( uint64_t type );
const char *ModuleBaseTypeStr( uint64_t modtype, uint64_t basetype );
const char *ModuleSubTypeStr( uint64_t modtype, uint64_t subtype );


enum ModuleType_: uint64_t
{
    ModuleType_Invalid = 0,
    ModuleType_Device,
    ModuleType_Daemon
};

// enum ModuleFlag_: uint64_t
// {
//     ModuleFlag_None = 0,
//     ModuleFlag_InterruptHandler = 1<<1
// };

enum DeviceType_: uint64_t
{
    DeviceType_Invalid = 0,
    DeviceType_Storage,
    DeviceType_Audio,
    DeviceType_Video,
    DeviceType_Keyboard,
    DeviceType_Mouse,
    DeviceType_Block   = 1<<15,
    DeviceType_Char    = 1<<16,
};

enum DaemonType_: uint64_t
{
    DaemonType_Invalid = 0,
    DaemonType_System,
    DaemonType_User,
};


#define MODULE_INTERFACE_HEADER \
uint64_t modtype        = ModuleType_Invalid;\
uint64_t basetype       = ModuleType_Invalid;\
uint64_t subtype        = ModuleType_Invalid;\
char     signature[8]   = {0,0,0,0,0,0,0,0};\
void     (*main)(void*) = nullptr;\

// struct ModuleHeader
// {
//     uint64_t modtype = ModuleType_Invalid;
//     uint64_t subtype = ModuleType_Invalid;
//     int64_t  isrno   = -1;
//     void    *isrfn   = nullptr;
//     char     signature[8];
// };


struct ModuleInterface
{
    MODULE_INTERFACE_HEADER
    uint8_t data[1];
};


struct KernelModule
{
    void *address;
    ModuleInterface *interface;
};


struct DeviceInterface
{
    MODULE_INTERFACE_HEADER
    void    (*open )();
    void    (*close)();
    void    *read;
    void    *write;
    int64_t  isrno;
    void    (*isrfn)(intframe_t*);
};

struct BlockDevInterface
{
    MODULE_INTERFACE_HEADER
    void     (*open )();
    void     (*close)();
    size_t   (*read )(void *dst, size_t offset, size_t nbytes);
    size_t   (*write)(const void *src, size_t offset, size_t nbytes);
    int64_t   isrno;
    void     (*isrfn)(intframe_t*);

};

struct CharDevInterface
{
    MODULE_INTERFACE_HEADER
    void     (*open )();
    void     (*close)();
    size_t   (*read )(void *dst, size_t nbytes);
    size_t   (*write)(const void *src, size_t nbytes);
    int64_t   isrno;
    void     (*isrfn)(intframe_t*);
};


struct DaemonInterface
{
    MODULE_INTERFACE_HEADER
    void (*start )();
    void (*stop  )();
    void (*reload)();
    int  (*status)(); // write status to stdout and return status code
};





// struct DevicePCIInformation
// {
//     uint16_t vendorID; // PCI Vendor ID
//     uint16_t deviceID; // PCI Device ID

//     uint8_t bus;  // PCI Bus
//     uint8_t slot; // PCI Slot
//     uint8_t func; // PCI Func

//     uint8_t classCode; // PCI Class
//     uint8_t subclass;  // PCI Subclass
//     uint8_t progIf;    // PCI Programming Interface

//     uintptr_t bars[6]; // PCI BARs
// };

// enum DeviceType {
//     DeviceTypeUnknown,
//     DeviceTypeDevFS,
//     DeviceTypeUNIXPseudo,
//     DeviceTypeUNIXPseudoTerminal,
//     DeviceTypeKernelLog,
//     DeviceTypeGenericPCI,
//     DeviceTypeGenericACPI,
//     DeviceTypeStorageController,
//     DeviceTypeStorageDevice,
//     DeviceTypeStoragePartition,
//     DeviceTypeNetworkStack,
//     DeviceTypeNetworkAdapter,
//     DeviceTypeUSBController,
//     DeviceTypeGenericUSB,
//     DeviceTypeLegacyHID,
//     DeviceTypeUSBHID,
//     DeviceTypeAudioController,
//     DeviceTypeAudioOutput,
// };

// enum DeviceBus {
//     DeviceBusNone,
//     DeviceBusSoftware,
//     DeviceBusPCI,
//     DeviceBusUSB,
// };

