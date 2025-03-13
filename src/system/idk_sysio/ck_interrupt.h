
#pragma once
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


// IRQ	Description
// 0	Programmable Interrupt Timer Interrupt
// 1	Keyboard Interrupt
// 2	Cascade (used internally by the two PICs. never raised)
// 3	COM2 (if enabled)
// 4	COM1 (if enabled)
// 5	LPT2 (if enabled)
// 6	Floppy Disk
// 7	LPT1 / Unreliable "spurious" interrupt (usually)
// 8	CMOS real-time clock (if enabled)
// 9	Free for peripherals / legacy SCSI / NIC
// 10	Free for peripherals / SCSI / NIC
// 11	Free for peripherals / SCSI / NIC
// 12	PS2 Mouse
// 13	FPU / Coprocessor / Inter-processor
// 14	Primary ATA Hard Disk
// 15	Secondary ATA Hard Disk





#ifdef __cplusplus
}
#endif

