#include <driver/pci.hpp>
#include <driver/pci_id.hpp>

#include <kmalloc.h>
#include <kernel/log.hpp>

#include <inplace_vector.hpp>
static PCI_Device PCI_devices_buf[128];
static idk::inplace_vector<PCI_Device> PCI_devices;


static uint8_t pci_read_byte( int bus, int slot, int fn, int off )
{
    uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
    IO::out32(PCI_CONFIG_ADDRESS, addr);
    return IO::in8(0xcfc + (off & 0x03));
}

static uint16_t pci_read_word( int bus, int slot, int fn, int off )
{
    if ((off & 0x03) == 0x3)
        return 0;
    uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
    IO::out32(PCI_CONFIG_ADDRESS, addr);
    return IO::in16(PCI_CONFIG_DATA + (off & 0x03));
}

static uint32_t pci_read_dword( int bus, int slot, int fn, int off )
{
    if (off & 0x03)
        return 0;
    
    uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
    IO::out32(PCI_CONFIG_ADDRESS, addr);
    return IO::in32(PCI_CONFIG_DATA);
}

// static void pci_write16( int bus, int slot, int fn, int off, uint16_t data )
// {
//     uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (fn << 8) | (off & 0xfc);
//     IO::out32(PCI_CONFIG_ADDRESS, addr);
//     IO::out32(PCI_CONFIG_DATA + (off & 0x03), data);
// }

void pci_write32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xfc) | 0x80000000);

    IO::out32(0xCF8, address);
    IO::out32(0xCFC, data);
}


#define PCI_GET_VENDOR(bus, dev, fun) pci_read_word(bus, dev, fun, PCI_VENDOR_ID)
#define PCI_GET_CLASS(bus, dev, fun) pci_read_byte(bus, dev, fun, PCI_CLASS)
#define PCI_GET_SUBCLASS(bus, dev, fun) pci_read_byte(bus, dev, fun, PCI_SUBCLASS)
#define PCI_GET_CLASSCODE(bus, dev, fun) ((pci_read_byte(bus, dev, fun, 0xB) << 8) | pci_read_byte(bus, dev, fun, 0xA))



/* https://www.reddit.com/r/osdev/comments/q83p87/pci_troubles/ */
// static void PCI_scan_function( uint32_t bus, uint32_t device, uint32_t function );
// static void PCI_scan_device( uint32_t bus, uint32_t device );
static void PCI_scan_bus( uint32_t bus );


static uint8_t getHeader( uint8_t bus, uint8_t slot, uint8_t fun )
{
    return pci_read_byte(bus, slot, fun, PCI_HEADER_TYPE);
}

static bool checkDevice( uint8_t bus, uint8_t slot, uint8_t fun )
{
    if (PCI_GET_VENDOR(bus, slot, fun) == PCI_NONE)
        return false;
    return true;
}


static int addDevice( uint8_t bus, uint8_t slot, uint8_t fun )
{
    PCI_devices.push_back(PCI_Device(bus, slot, fun));
    return PCI_devices.size() - 1;
}


// static void PCI_scan_function( uint32_t bus, uint32_t dev, uint32_t function )
// {
//     uint32_t header    = pci_read_byte(bus, dev, function, PCI_HEADER_TYPE);
//     uint32_t baseclass = PCI_GET_CLASS(bus, dev, function);
//     uint32_t subclass  = PCI_GET_SUBCLASS(bus, dev, function);

//     syslog log("%s (0x%x)", baseclass_str(baseclass), baseclass);
//     log("header:   0x%x", header);
//     log("function: %u", function);
//     log("subclass: %s (0x%x)", subclass_str(baseclass, subclass), subclass);

//     if ((baseclass == PCI::CLASS_BRIDGE) && (subclass == PCI::BRIDGE_PCI))
//     {
//         uint8_t secondary_bus = pci_read_byte(bus, dev, function, PCI_SECONDARY_BUS);
//         log("secondary_bus: 0x%x", secondary_bus);
//         PCI_scan_bus(secondary_bus);
//     }

//     else
//     {
//         PCI_Device device(bus, dev, function);
//         // PCI_Devices.push_back(PCI_dev(bus, dev, function));
//     }
// }


// static void PCI_scan_device( uint32_t bus, uint32_t dev )
// {
//     /*
//         Scan function 0 for a valid vendor ID first. If it's valid, bit 7 of the header
//         type will indicate whether functions 1 through 7 will return sensible data.
//     */
//     uint16_t vendor = pci_read_word(bus, dev, 0, PCI_VENDOR_ID);
//     if (vendor == PCI_NONE)
//         return;

//     syslog log("%s (0x%x)", vendorid_str(vendor), vendor);
//     PCI_scan_function(bus, dev, 0);

//     uint32_t header = pci_read_byte(bus, dev, 0, PCI_HEADER_TYPE);

//     if (header & 0x80)
//     {
//         for (uint32_t fun=1; fun<8; fun++)
//         {
//             if (PCI_GET_VENDOR(bus, dev, fun) == PCI_NONE)
//                 continue;
//             PCI_scan_function(bus, dev, fun);
//         }
//     }
// }


static void PCI_scan_slot( uint32_t bus, uint32_t slot )
{
    if (!checkDevice(bus, slot, 0))
    {
        return;
    }

    addDevice(bus, slot, 0);

    if (!(0x80 & pci_read_byte(bus, slot, 0, PCI_HEADER_TYPE)))
    {
        return;
    }

    for (uint8_t fn=0; fn<8; fn++)
    {
        if (!checkDevice(bus, slot, fn))
            continue;
        addDevice(bus, slot, fn);
    
        uint8_t baseclass = PCI_GET_CLASS(bus, slot, fn);
        uint8_t subclass  = PCI_GET_SUBCLASS(bus, slot, fn);
    
        if ((baseclass == PCI::CLASS_BRIDGE) && (subclass == PCI::BRIDGE_PCI))
        {
            uint8_t secondaryBus = pci_read_byte(bus, slot, fn, PCI_SECONDARY_BUS);
            PCI_scan_bus(secondaryBus);
        }
    }
}

static void PCI_scan_bus( uint32_t bus )
{
    for (uint32_t slot=0; slot<32; slot++)
    {
        PCI_scan_slot(bus, slot);
    }
}


void PCI::init()
{
    syslog log("PCI_init");

    PCI_devices = idk::inplace_vector<PCI_Device>(
        PCI_devices_buf, 128
    );


    uint8_t header = getHeader(0, 0, 0);

    // Single PCI host controller
    if ((header & 0x80) == 0)
    {
        PCI_scan_bus(0);
    }

    // Multiple PCI host controllers
    else
    {
        for (uint8_t fn=0; fn<8; fn++)
        {
            if (PCI_GET_VENDOR(0, 0, fn) != 0xFFFF)
                break;
            PCI_scan_bus(fn);
        }
    }

}


// PCI_dev PCI::findDevice( uint16_t vendor_id, uint16_t device_id )
// {
//     for (auto &dev: PCI_Devices)
//     {
//         if ((dev.vendor_id == vendor_id) && (dev.device_id == device_id))
//         {
//             return dev;
//         }
//     }

//     PCI_dev dummy;
//     dummy.vendor_id = PCI_NONE;
//     dummy.device_id = PCI_NONE;
//     return dummy;
// }



// static void PCI_readBar( uint8_t bus, uint8_t slot, uint8_t fn, uint32_t index, uint32_t *address, uint32_t *mask)
// {
//     uint32_t reg = PCI_BAR0 + index * sizeof(uint32_t);

//     // Get address
//     *address = pci_read_word(bus, slot, fn, reg);

//     // Find out size of the bar
//     pci_write32(bus, slot, fn, reg, 0xffffffff);
//     *mask = pci_read_dword(bus, slot, fn, reg);

//     // Restore adddress
//     pci_write32(bus, slot, fn, reg, *address);
// }


// static void PCI_getBar( PCIBarInfo *bar, uint8_t bus, uint8_t slot, uint8_t fn, uint32_t index )
// {
//     // Read pci bar register
//     uint32_t addressLow;
//     uint32_t maskLow;
//     PCI_readBar(bus, slot, fn, index, &addressLow, &maskLow);

//     if (addressLow & PCI_BAR_64)
//     {
//         // 64-bit mmio
//         uint32_t addressHigh;
//         uint32_t maskHigh;
//         PCI_readBar(bus, slot, fn, index + 1, &addressHigh, &maskHigh);

//         bar->u.address = (void *)(((uintptr_t)addressHigh << 32) | (addressLow & ~0xf));
//         bar->size = ~(((uint64_t)maskHigh << 32) | (maskLow & ~0xf)) + 1;
//         bar->flags = addressLow & 0xf;
//     }
//     else if (addressLow & PCI_BAR_IO)
//     {
//         // i/o register
//         bar->u.port = (uint16_t)(addressLow & ~0x3);
//         bar->size = (uint16_t)(~(maskLow & ~0x3) + 1);
//         bar->flags = addressLow & 0x3;
//     }
//     else
//     {
//         // 32-bit mmio
//         bar->u.address = (void *)(uintptr_t)(addressLow & ~0xf);
//         bar->size = ~(maskLow & ~0xf) + 1;
//         bar->flags = addressLow & 0xf;
//     }
// }






PCI_Device::PCI_Device()
: vendor_id(PCI_NONE), device_id(PCI_NONE) {  }


PCI_Device::PCI_Device( uint16_t bs, uint16_t st, uint16_t fn )
{
    vendor_id = pci_read_word(bs, st, fn, PCI_VENDOR_ID);
    device_id = pci_read_word(bs, st, fn, PCI_DEVICE_ID);
    bus       = bs;
    slot      = st;
    func      = fn;
    baseclass = PCI_GET_CLASS(bs, st, fn);
    subclass  = PCI_GET_SUBCLASS(bs, st, fn);

    syslog log("%s", subclass_str(baseclass, subclass), subclass);
    log("vendor:   %s (0x%x)", vendorid_str(vendor_id), vendor_id);
    log("device:   %s (0x%x)", deviceid_str(device_id), device_id);
    log("class:    %s (0x%x)", baseclass_str(baseclass), baseclass);
    log("subclass: %s (0x%x)", subclass_str(baseclass, subclass), subclass);

    for (int i=0; i<6; i++)
    {
        union {
            uint32_t bar;
            PCIBarInfo binfo;
        };
    
        bar = pci_read_dword(bs, st, fn, PCI_BAR0 + 4*i);

        if (bar == 0)
        {
            continue;
        }
    
        syslog lg("bar%d", i);

        if (binfo.is_iospace)
        {
            auto &info = binfo.iospace;
            lg("- iospace");
            lg("- addr:  0x%lx", info.address);
        }
    
        else
        {
            auto &info = binfo.memspace;
            lg("- memspace");
            lg("- addr:     0x%lx", info.address);
            lg("- prefetch: %u", info.prefetchable);
        }

    }

}



