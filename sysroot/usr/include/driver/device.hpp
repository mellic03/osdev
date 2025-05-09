#pragma once
#include <stddef.h>
#include <stdint.h>
#include <kernel/interrupt.hpp>


using DevMain_t  = void (*)(void*);
using DevOpen_t  = void (*)(void*);
using DevClose_t = void (*)(void);
using DevRead_t  = size_t (*)(void*, size_t);
using DevWrite_t = size_t (*)(const void*, size_t);
using DevIrq_t   = void(*)(intframe_t*);

enum DevInterface_: uint64_t
{
    DevInterface_Invalid = 0,
    DevInterface_Block,
    DevInterface_Char
};


struct DeviceInterface
{
    uint64_t    type;
    char        signature[8];
    DevMain_t   main;
    DevOpen_t   open;
    DevClose_t  close;
    DevRead_t   read;
    DevWrite_t  write;
    DevIrq_t    irq;
    uint16_t    irqno;
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

