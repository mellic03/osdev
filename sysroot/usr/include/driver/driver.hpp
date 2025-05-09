#pragma once
#include <stddef.h>
#include <stdint.h>
#include <kernel/interrupt.hpp>

struct ksym_t;


using DevMain_t  = void (*)(void*);
using DevInit_t  = void (*)(ksym_t*);
using DevClose_t = void (*)(void);
using DevRead_t  = size_t (*)(void*, size_t);
using DevWrite_t = size_t (*)(const void*, size_t);
using DevIrq_t   = void(*)(intframe_t*);


namespace KernelModule
{
    enum Interface_: uint64_t
    {
        Interface_Invalid =0,
        Interface_Device,
        Interface_Service
    };

    enum Device_: uint64_t
    {
        Device_Invalid =0,
        Device_Storage,
        Device_Audio,
        Device_Video,
        Device_Keyboard,
        Device_Mouse,
    };

    enum Service_: uint64_t
    {
        Service_Invalid =0,
        Service_System,
        Service_User,
    };
}



// struct ModuleInterface
// {
//     uint64_t interface_type; // = KernelModule::Interface_Invalid;
//     char     signature[8];

// };




enum DevInterface_: uint64_t
{
    DevInterface_Invalid = 0,
    DevInterface_Block,
    DevInterface_Char
};


// struct DriverInterface
struct ModuleInterface
{
    uint64_t    type;
    char        signature[8];
    DevMain_t   main;
    DevInit_t   init;
    DevClose_t  close;
    DevRead_t   read;
    DevWrite_t  write;
    DevIrq_t    irq;
    uint64_t    irqno;
};


struct BlockDeviceInterface
{
    uint64_t  type;
    char      signature[8];
    uint64_t  pad0;
    void     (*open )(ksym_t*);
    void     (*close)(void);
    size_t   (*read )(void *dst, size_t offset, size_t nbytes);
    size_t   (*write)(const void *src, size_t offset, size_t nbytes);
    uint64_t  pad1;
    uint64_t  pad2;
};


struct CharDeviceInterface
{
    uint64_t  type;
    char      signature[8];
    uint64_t  pad0;
    void     (*open )(ksym_t*);
    void     (*close)(void);
    size_t   (*read )(void *dst, size_t offset, size_t nbytes);
    size_t   (*write)(const void *src, size_t offset, size_t nbytes);
    uint64_t  pad1;
    uint64_t  pad2;
};


// void rere()
// {
//     sizeof(DriverInterface);
//     sizeof(BlockDeviceInterface);
// }



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

