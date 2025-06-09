#include <driver/pci.hpp>
#include <driver/pci_id.hpp>

#include <kernel/log.hpp>
#include <kmalloc.h>

#include <bitmap.hpp>
#include <inplace_vector.hpp>


static PCI_Device *PCI_RootDevice; //      = PCI_Device(PCIAddress(0, 0, 0));
static auto PCI_DeviceAllocator = idk::BitMapAllocator2<PCI_Device, 128>();

// static PCI_Device *PCI_devices_buf[128];
// static idk::inplace_vector<PCI_Device*> PCI_devices;

static constexpr uint8_t PCI_GET_HEADER( uint8_t bus, uint8_t dev, uint8_t fun )
{ return PCI::read8(PCIAddress(bus, dev, fun), PCI_HEADER_TYPE); }

static constexpr uint16_t PCI_GET_VENDOR( uint8_t bus, uint8_t dev, uint8_t fun )
{ return PCI::read16(PCIAddress(bus, dev, fun), PCI_VENDOR_ID); }

static constexpr uint8_t PCI_GET_CLASS( uint8_t bus, uint8_t dev, uint8_t fun )
{ return PCI::read8(PCIAddress(bus, dev, fun), PCI_CLASS); }

static constexpr uint8_t PCI_GET_SUBCLASS( uint8_t bus, uint8_t dev, uint8_t fun )
{ return PCI::read8(PCIAddress(bus, dev, fun), PCI_SUBCLASS); }


/* https://www.reddit.com/r/osdev/comments/q83p87/pci_troubles/ */
static void PCI_scan_bus( PCI_Device *pdev, uint32_t bus );

static bool checkDevice( uint8_t bus, uint8_t slot, uint8_t fun )
{
    if (PCI_GET_VENDOR(bus, slot, fun) == PCI_NONE)
        return false;
    return true;
}


// static int addDevice( uint8_t bus, uint8_t slot, uint8_t fun )
// {
//     PCIAddress addr(bus, slot, fun);
//     PCI_devices.push_back(PCI_Device(addr));
//     return PCI_devices.size() - 1;
// }



static PCI_Device *PCI_scan_device( uint32_t bus, uint32_t dev )
{
    if (!checkDevice(bus, dev, 0))
    {
        return nullptr;
    }

    auto *currdev = PCI_DeviceAllocator.alloc(PCIAddress(bus, dev, 0));
    // PCI_devices.push_back(currdev);

    // PCI_Device *childA;

    // if (PCIAddress(bus, dev, 0).dword == currdev->address.dword)
    // {
    //     childA = currdev;
    // }

    // else
    // {
    //     childA = PCI_DeviceAllocator.alloc(PCIAddress(bus, dev, 0));
    //     currdev->children.insert(childA);
    //     PCI_devices.push_back(childA);
    // }

    // addDevice(bus, dev, 0);

    if (!(0x80 & PCI_GET_HEADER(bus, dev, 0)))
    {
        return currdev;
    }

    for (uint8_t fn=0; fn<8; fn++)
    {
        if (!checkDevice(bus, dev, fn))
            continue;

        auto *child = PCI_DeviceAllocator.alloc(PCIAddress(bus, dev, fn));
        currdev->children.insert(child);
        // PCI_devices.push_back(child);
    
        uint8_t baseclass = PCI_GET_CLASS(bus, dev, fn);
        uint8_t subclass  = PCI_GET_SUBCLASS(bus, dev, fn);
    
        if ((baseclass == PCI::CLASS_BRIDGE) && (subclass == PCI::BRIDGE_PCI))
        {
            uint8_t secondaryBus = PCI::read8(PCIAddress(bus, dev, fn), PCI_SECONDARY_BUS);
            PCI_scan_bus(child, secondaryBus);
        }
    }

    return currdev;
}


static void PCI_scan_bus( PCI_Device *currdev, uint32_t bus )
{
    for (uint32_t dev=0; dev<32; dev++)
    {
        auto *child = PCI_scan_device(bus, dev);

        if (child)
        {
            currdev->children.insert(child);
        }
    }
}




static void PCI_PrintDevices( PCI_Device *curr )
{
    static int indent0 = syslog::getIndent();
    static int indent  = indent0;
    for (int i=0; i<indent-1; i++)
        syslog::print(" ");
    syslog::print("- ");

    PCIAddress addr = curr->address;
    syslog::println(
        "[PCI %u:%u.%u] %s %s",
        addr.bus, addr.device, addr.func,
        subclass_str(curr->baseclass, curr->subclass) + sizeof("PCI_CLASS_")-1,
        deviceid_str(curr->device_id) + sizeof("PCI_DEVICE_ID_")-1
    );

    for (auto *child: curr->children)
    {
        indent += 2;
        PCI_PrintDevices((PCI_Device*)child);
        indent -= 2;
    }
}



// static void PCI_PrintDevices( PCI_Device *curr )
// {
//     static int indent0 = syslog::getIndent();
//     static int indent  = indent0;

//     for (auto *child: curr->children)
//     {
//         PCIAddress addr = child->address;

//         for (int i=0; i<indent-1; i++)
//             syslog::print(" ");
//         syslog::print("- ");

//         syslog::println(
//             "[PCI %u:%u.%u] %s %s",
//             addr.bus, addr.device, addr.func,
//             subclass_str(child->baseclass, child->subclass) + sizeof("PCI_CLASS_")-1,
//             deviceid_str(child->device_id) + sizeof("PCI_DEVICE_ID_")-1
//         );

//         indent += 2;
//         PCI_PrintDevices((PCI_Device*)child);
//         indent -= 2;
//     }
// }




void PCI::init()
{
    syslog log("PCI_init");
    PCI_RootDevice = PCI_DeviceAllocator.alloc(PCIAddress(0, 0, 0));

    if ((PCI_GET_HEADER(0, 0, 0) & 0x80) == 0)
    {
        log("Single PCI host controller");
        PCI_scan_bus(PCI_RootDevice, 0);
    }

    else
    {
        log("Multiple PCI host controllers");

        for (uint8_t fn=0; fn<8; fn++)
        {
            if (PCI_GET_VENDOR(0, 0, fn) != 0xFFFF)
                break;
            PCI_scan_bus(PCI_RootDevice, fn);
        }
    }

    PCI_PrintDevices(PCI_RootDevice);
}

static PCI_Device *PCI_findDevice( PCI_Device *curr, uint16_t vid, uint16_t did )
{
    if (curr->vendor_id == vid && curr->device_id == did)
        return curr;

    for (auto *childA: curr->children)
    {
        auto *childB = PCI_findDevice((PCI_Device*)childA, vid, did);
        if (childB)
            return childB;
    }

    return nullptr;
}


PCI_Device *PCI::findDevice( uint16_t vendor_id, uint16_t device_id )
{
    return PCI_findDevice(PCI_RootDevice, vendor_id, device_id);
}






uint32_t PCI::getBARAddr( PCIAddress addr, int idx )
{
    uint32_t bar = PCI::read32(addr, PCI_BAR0 + 4*idx);
    uint32_t mask = (bar & PCI_BAR_IO) ? 0x3 : 0xf;
    return bar & ~mask;
}



PCI_Device::PCI_Device( const PCIAddress &addr )
:   address(addr),
    vendor_id (PCI::read16(addr, PCI_VENDOR_ID)),
    device_id (PCI::read16(addr, PCI_DEVICE_ID)),
    baseclass (PCI::read8 (addr, PCI_CLASS)),
    subclass  (PCI::read8 (addr, PCI_SUBCLASS))
{
    uint8_t bs = addr.bus;
    uint8_t st = addr.device;
    uint8_t fn = addr.func;

    syslog log("%u:%u.%u  %s", bs, st, fn, deviceid_str(device_id));
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

        bar = PCI::read32(addr, PCI_BAR0 + 4*i);
        this->bars[i] = binfo;

        if (bar == 0)
            continue;

        syslog lg("bar%d", i, bar);

        if (binfo.is_iospace)
        {
            auto &info = binfo.iospace;
            info.address = PCI::getBARAddr(addr, i);
            lg("- iospace");
            lg("- addr:  0x%lx", info.address);
        }
    
        else
        {
            auto &info = binfo.memspace;
            info.address = PCI::getBARAddr(addr, i);
            lg("- memspace");
            lg("- addr:     0x%lx", info.address);
            lg("- prefetch: %u", info.prefetchable);
        }

    }

}













uint8_t PCI::read8( PCIAddress addr, uint8_t offset )
{
    addr.enable = 1; addr.offset = offset;
    IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
    return IO::in8(PCI_CONFIG_DATA);
}

uint16_t PCI::read16( PCIAddress addr, uint8_t offset )
{
    addr.enable = 1; addr.offset = offset;
    IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
    return IO::in16(PCI_CONFIG_DATA);
}

uint32_t PCI::read32( PCIAddress addr, uint8_t offset )
{
    addr.enable = 1; addr.offset = offset;
    IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
    return IO::in32(PCI_CONFIG_DATA);
}


void PCI::write8( PCIAddress addr, uint8_t offset, uint8_t value )
{
    addr.enable = 1; addr.offset = offset;
    IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
    IO::out8(PCI_CONFIG_DATA, value);
}

void PCI::write16( PCIAddress addr, uint8_t offset, uint16_t value )
{
    addr.enable = 1; addr.offset = offset;
    IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
    IO::out16(PCI_CONFIG_DATA, value);
}

void PCI::write32( PCIAddress addr, uint8_t offset, uint32_t value )
{
    addr.enable = 1; addr.offset = offset;
    IO::out32(PCI_CONFIG_ADDRESS, addr.dword);
    IO::out32(PCI_CONFIG_DATA, value);
}

