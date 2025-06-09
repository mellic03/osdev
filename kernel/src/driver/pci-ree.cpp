// #include <driver/pci.hpp>
// #include <driver/pci_id.hpp>

// #include <kernel/log.hpp>
// #include <kmalloc.h>

// #include <bitmap.hpp>
// #include <inplace_vector.hpp>


// static PCI_Device *PCI_RootDevice; //      = PCI_Device(PCIAddress(0, 0, 0));
// static auto PCI_DeviceAllocator = idk::BitMapAllocator2<PCI_Device, 128>();

// static PCI_Device PCI_devices_buf[128];
// static idk::inplace_vector<PCI_Device> PCI_devices;



// static uint8_t pci_read_byte( int bus, int slot, int fn, int off )
// {
//     uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
//     IO::out32(PCI_CONFIG_ADDRESS, addr);
//     return IO::in8(0xcfc + (off & 0x03));
// }

// static uint16_t pci_read_word( int bus, int slot, int fn, int off )
// {
//     if ((off & 0x03) == 0x3)
//         return 0;
//     uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
//     IO::out32(PCI_CONFIG_ADDRESS, addr);
//     return IO::in16(PCI_CONFIG_DATA + (off & 0x03));
// }

// static uint32_t pci_read_dword( int bus, int slot, int fn, int off )
// {
//     if (off & 0x03)
//         return 0;
    
//     uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
//     IO::out32(PCI_CONFIG_ADDRESS, addr);
//     return IO::in32(PCI_CONFIG_DATA);
// }






// uint16_t PCI::read16( int bus, int slot, int fn, int off )
// {
//     if ((off & 0x03) == 0x3)
//         return 0;
//     uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
//     IO::out32(PCI_CONFIG_ADDRESS, addr);
//     return IO::in16(PCI_CONFIG_DATA + (off & 0x03));
// }


// void PCI::write16( int bus, int slot, int fn, int off, uint16_t data )
// {
//     uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
//     IO::out32(PCI_CONFIG_ADDRESS, addr);
//     IO::out32(PCI_CONFIG_DATA + (off & 0x03), data);
// }

// void pci_write32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data)
// {
//     uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xfc) | 0x80000000);
//     IO::out32(0xCF8, address);
//     IO::out32(0xCFC, data);
// }



// uint8_t PCI::read8( PCIAddress addr, uint8_t offset )
// {
//     if ((offset & 0x03) == 0x3)
//         return 0;
//     addr.enable = 1; addr.offset = offset;
//     IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
//     return IO::in8(PCI_CONFIG_DATA + (offset & 0x03));
// }

// uint16_t PCI::read16( PCIAddress addr, uint8_t offset )
// {
//     if ((offset & 0x03) == 0x3)
//         return 0;
//     addr.enable = 1; addr.offset = offset;
//     IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
//     return IO::in16(PCI_CONFIG_DATA + (offset & 0x03));
// }

// uint32_t PCI::read32( PCIAddress addr, uint8_t offset )
// {
//     if ((offset & 0x03))
//         return 0;
//     addr.enable = 1; addr.offset = offset;
//     IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
//     return IO::in32(PCI_CONFIG_DATA + (offset & 0x03));
// }

// void PCI::write16( PCIAddress addr, uint8_t offset, uint16_t value )
// {
//     addr.enable = 1; addr.offset = offset;
//     IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
//     IO::out16(PCI_CONFIG_DATA + (offset & 0x03), value);
// }

// void PCI::write32( PCIAddress addr, uint8_t offset, uint32_t value )
// {
//     addr.enable = 1; addr.offset = offset;
//     IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
//     IO::out32(PCI_CONFIG_DATA + (offset & 0x03), value);
// }


// // static constexpr uint8_t PCI_GET_HEADER( uint8_t bus, uint8_t dev, uint8_t fun )
// // { return PCI::read8(PCIAddress(bus, dev, fun), PCI_HEADER_TYPE); }


// #define PCI_GET_VENDOR(bus, dev, fun) pci_read_word(bus, dev, fun, PCI_VENDOR_ID)
// #define PCI_GET_CLASS(bus, dev, fun) pci_read_byte(bus, dev, fun, PCI_CLASS)
// #define PCI_GET_SUBCLASS(bus, dev, fun) pci_read_byte(bus, dev, fun, PCI_SUBCLASS)
// #define PCI_GET_CLASSCODE(bus, dev, fun) ((pci_read_byte(bus, dev, fun, 0xB) << 8) | pci_read_byte(bus, dev, fun, 0xA))

// // static constexpr uint16_t PCI_GET_VENDOR( uint8_t bus, uint8_t dev, uint8_t fun )
// // { return PCI::read16(PCIAddress(bus, dev, fun), PCI_VENDOR_ID); }

// // static constexpr uint8_t PCI_GET_CLASS( uint8_t bus, uint8_t dev, uint8_t fun )
// // { return PCI::read8(PCIAddress(bus, dev, fun), PCI_CLASS); }

// // static constexpr uint8_t PCI_GET_SUBCLASS( uint8_t bus, uint8_t dev, uint8_t fun )
// // { return PCI::read8(PCIAddress(bus, dev, fun), PCI_SUBCLASS); }


// /* https://www.reddit.com/r/osdev/comments/q83p87/pci_troubles/ */
// // static void PCI_scan_function( uint32_t bus, uint32_t device, uint32_t function );
// // static void PCI_scan_device( uint32_t bus, uint32_t device );
// static void PCI_scan_bus( uint32_t bus );

// static bool checkDevice( uint8_t bus, uint8_t slot, uint8_t fun )
// {
//     if (PCI_GET_VENDOR(bus, slot, fun) == PCI_NONE)
//         return false;
//     return true;
// }


// static int addDevice( uint8_t bus, uint8_t slot, uint8_t fun )
// {
//     PCIAddress addr(bus, slot, fun);
//     PCI_devices.push_back(PCI_Device(addr));
//     return PCI_devices.size() - 1;
// }



// static void PCI_scan_device( uint32_t bus, uint32_t dev )
// {
//     if (!checkDevice(bus, dev, 0))
//     {
//         return;
//     }

//     addDevice(bus, dev, 0);

//     if (!(0x80 & pci_read_byte(bus, dev, 0, PCI_HEADER_TYPE)))
//     // if (!(0x80 & PCI_GET_HEADER(bus, dev, 0)))
//     {
//         return;
//     }

//     for (uint8_t fn=0; fn<8; fn++)
//     {
//         if (!checkDevice(bus, dev, fn))
//             continue;
//         addDevice(bus, dev, fn);
    
//         uint8_t baseclass = PCI_GET_CLASS(bus, dev, fn);
//         uint8_t subclass  = PCI_GET_SUBCLASS(bus, dev, fn);
    
//         if ((baseclass == PCI::CLASS_BRIDGE) && (subclass == PCI::BRIDGE_PCI))
//         {
//             uint8_t secondaryBus = pci_read_byte(bus, dev, fn, PCI_SECONDARY_BUS);
//             // uint8_t secondaryBus = PCI::read8(PCIAddress(bus, dev, fn), PCI_SECONDARY_BUS);
//             PCI_scan_bus(secondaryBus);
//         }
//     }
// }

// static void PCI_scan_bus( uint32_t bus )
// {
//     for (uint32_t dev=0; dev<32; dev++)
//     {
//         PCI_scan_device(bus, dev);
//     }
// }




// void PCI::init()
// {
//     syslog log("PCI_init");

//     PCI_RootDevice = PCI_DeviceAllocator.alloc();

//     PCI_devices = idk::inplace_vector<PCI_Device>(
//         PCI_devices_buf, 128
//     );

//     uint8_t header = pci_read_byte(0, 0, 0, PCI_HEADER_TYPE);

//     // Single PCI host controller
//     if ((header & 0x80) == 0)
//     {
//         log("Single PCI host controller");
//         PCI_scan_bus(0);
//     }

//     // Multiple PCI host controllers
//     else
//     {
//         log("Multiple PCI host controllers");

//         for (uint8_t fn=0; fn<8; fn++)
//         {
//             if (PCI_GET_VENDOR(0, 0, fn) != 0xFFFF)
//                 break;
//             PCI_scan_bus(fn);
//         }
//     }

// }


// PCI_Device *PCI::findDevice( uint16_t vendor_id, uint16_t device_id )
// {
//     for (auto &dev: PCI_devices)
//     {
//         if ((dev.vendor_id == vendor_id) && (dev.device_id == device_id))
//         {
//             return &dev;
//         }
//     }

//     return nullptr;
// }





// uint32_t PCI::getBARAddr( PCIAddress addr, int idx )
// {
//     // uint32_t bar = PCI::read32(addr, PCI_BAR0 + idx*sizeof(uint32_t));
//     uint32_t bar = pci_read_dword(addr.bus, addr.device, addr.func, PCI_BAR0 + 4*idx);
//     uint32_t mask = (bar & PCI_BAR_IO) ? 0x3 : 0xf;
//     return bar & ~mask;

//     // uint32_t mask = (bar & PCI_BAR_IO) ? 0b1111'1100 : 0b1111'0000;
//     // return bar & mask;
// }



// PCI_Device::PCI_Device( const PCIAddress &addr )
// :   address(addr),
//     vendor_id (pci_read_word(addr.bus, addr.device, addr.func, PCI_VENDOR_ID)),
//     device_id (pci_read_word(addr.bus, addr.device, addr.func, PCI_DEVICE_ID)),
//     baseclass (pci_read_byte(addr.bus, addr.device, addr.func, PCI_CLASS)),
//     subclass  (pci_read_byte(addr.bus, addr.device, addr.func, PCI_SUBCLASS))
// {
//     uint8_t bs = addr.bus;
//     uint8_t st = addr.device;
//     uint8_t fn = addr.func;

//     syslog log("%u:%u.%u  %s", bs, st, fn, deviceid_str(device_id));
//     log("vendor:   %s (0x%x)", vendorid_str(vendor_id), vendor_id);
//     log("device:   %s (0x%x)", deviceid_str(device_id), device_id);
//     log("class:    %s (0x%x)", baseclass_str(baseclass), baseclass);
//     log("subclass: %s (0x%x)", subclass_str(baseclass, subclass), subclass);

//     for (int i=0; i<6; i++)
//     {
//         union {
//             uint32_t bar;
//             PCIBarInfo binfo;
//         };

//         bar = pci_read_dword(addr.bus, addr.device, addr.func, PCI_BAR0 + 4*i);
//         // bar = PCI::read32(addr, PCI_BAR0 + 4*i);
//         this->bars[i] = binfo;

//         if (bar == 0)
//             continue;

//         syslog lg("bar%d", i, bar);

//         if (binfo.is_iospace)
//         {
//             auto &info = binfo.iospace;
//             info.address = PCI::getBARAddr(addr, i);
//             lg("- iospace");
//             lg("- addr:  0x%lx", info.address);
//         }
    
//         else
//         {
//             auto &info = binfo.memspace;
//             info.address = PCI::getBARAddr(addr, i);
//             lg("- memspace");
//             lg("- addr:     0x%lx", info.address);
//             lg("- prefetch: %u", info.prefetchable);
//         }

//     }

// }



