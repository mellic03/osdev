// #include <driver/interface.hpp>
// #include <sym/sym.hpp>

// #include <arch/io.hpp>
// #include <kernel/log.hpp>
// #include <kernel/event.hpp>
// #include <sys/interrupt.hpp>
// #include <kernel/ringbuffer.hpp>
// #include <kernel/kscancode.h>
// #include <kmemxx.hpp>
// #include <algorithm>
// #include <ctype.h>


// typedef volatile struct tagHBA_PORT
// {
// 	uint32_t clb;		// 0x00, command list base address, 1K-byte aligned
// 	uint32_t clbu;		// 0x04, command list base address upper 32 bits
// 	uint32_t fb;		// 0x08, FIS base address, 256-byte aligned
// 	uint32_t fbu;		// 0x0C, FIS base address upper 32 bits
// 	uint32_t is;		// 0x10, interrupt status
// 	uint32_t ie;		// 0x14, interrupt enable
// 	uint32_t cmd;		// 0x18, command and status
// 	uint32_t rsv0;		// 0x1C, Reserved
// 	uint32_t tfd;		// 0x20, task file data
// 	uint32_t sig;		// 0x24, signature
// 	uint32_t ssts;		// 0x28, SATA status (SCR0:SStatus)
// 	uint32_t sctl;		// 0x2C, SATA control (SCR2:SControl)
// 	uint32_t serr;		// 0x30, SATA error (SCR1:SError)
// 	uint32_t sact;		// 0x34, SATA active (SCR3:SActive)
// 	uint32_t ci;		// 0x38, command issue
// 	uint32_t sntf;		// 0x3C, SATA notification (SCR4:SNotification)
// 	uint32_t fbs;		// 0x40, FIS-based switch control
// 	uint32_t rsv1[11];	// 0x44 ~ 0x6F, Reserved
// 	uint32_t vendor[4];	// 0x70 ~ 0x7F, vendor specific
// } HBA_PORT;


// typedef volatile struct tagHBA_MEM
// {
// 	// 0x00 - 0x2B, Generic Host Control
// 	uint32_t cap;		// 0x00, Host capability
// 	uint32_t ghc;		// 0x04, Global host control
// 	uint32_t is;		// 0x08, Interrupt status
// 	uint32_t pi;		// 0x0C, Port implemented
// 	uint32_t vs;		// 0x10, Version
// 	uint32_t ccc_ctl;	// 0x14, Command completion coalescing control
// 	uint32_t ccc_pts;	// 0x18, Command completion coalescing ports
// 	uint32_t em_loc;		// 0x1C, Enclosure management location
// 	uint32_t em_ctl;		// 0x20, Enclosure management control
// 	uint32_t cap2;		// 0x24, Host capabilities extended
// 	uint32_t bohc;		// 0x28, BIOS/OS handoff control and status

// 	// 0x2C - 0x9F, Reserved
// 	uint8_t  rsv[0xA0-0x2C];

// 	// 0xA0 - 0xFF, Vendor specific registers
// 	uint8_t  vendor[0x100-0xA0];

// 	// 0x100 - 0x10FF, Port control registers
// 	HBA_PORT	ports[1];	// 1 ~ 32
// } HBA_MEM;



// void irq_handler( intframe_t* )
// {

// }


// #include <filesystem/ramfs.hpp>

// static void ahci_open()
// {
//     // keypipe = usrknl::popen("/dev/kb0", sizeof(knl::KbEvent));
// }


// void ahci_main( void* )
// {
//     while (true)
//     {

//         kthread::yield();
//     }
// }



// extern "C"
// ModuleInterface *init( ksym::ksym_t *sym )
// {
//     ksym::loadsym(sym);

//     auto *kbdev = (CharDevInterface*)std::malloc(sizeof(CharDevInterface));

//     *kbdev = {
//         .modtype  = ModuleType_Device,
//         .basetype = DeviceType_Storage,
//         .main     = ahci_main,

//         .open     = ahci_open,
//         .close    = nullptr,
//         .read     = nullptr,
//         .write    = nullptr,
//         .irqno    = IrqNo_Keyboard,
//         .irqfn    = irq_handler,
//     };

//     kmemset<char>(kbdev->signature, '\0', sizeof(kbdev->signature));
//     kmemcpy<char>(kbdev->signature, "kboard", 6);

//     return (ModuleInterface*)kbdev;
// }

