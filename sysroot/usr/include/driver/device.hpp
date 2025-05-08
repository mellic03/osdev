#pragma once
#include <stdint.h>

struct DevicePCIInformation
{
    uint16_t vendorID; // PCI Vendor ID
    uint16_t deviceID; // PCI Device ID

    uint8_t bus;  // PCI Bus
    uint8_t slot; // PCI Slot
    uint8_t func; // PCI Func

    uint8_t classCode; // PCI Class
    uint8_t subclass;  // PCI Subclass
    uint8_t progIf;    // PCI Programming Interface

    uintptr_t bars[6]; // PCI BARs
};

enum DeviceType {
    DeviceTypeUnknown,
    DeviceTypeDevFS,
    DeviceTypeUNIXPseudo,
    DeviceTypeUNIXPseudoTerminal,
    DeviceTypeKernelLog,
    DeviceTypeGenericPCI,
    DeviceTypeGenericACPI,
    DeviceTypeStorageController,
    DeviceTypeStorageDevice,
    DeviceTypeStoragePartition,
    DeviceTypeNetworkStack,
    DeviceTypeNetworkAdapter,
    DeviceTypeUSBController,
    DeviceTypeGenericUSB,
    DeviceTypeLegacyHID,
    DeviceTypeUSBHID,
    DeviceTypeAudioController,
    DeviceTypeAudioOutput,
};

enum DeviceBus {
    DeviceBusNone,
    DeviceBusSoftware,
    DeviceBusPCI,
    DeviceBusUSB,
};

