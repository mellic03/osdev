// #include <kdriver/serial.hpp>

// using namespace idk;

// static void* isr[256] = { NULL };

// void isr_init()
// {
// 	// Remap the PIC to from 0..15 to 32..47
// 	IO::outb(0x20, 0x11);
// 	IO::outb(0xA0, 0x11);
// 	IO::outb(0x21, 0x20);
// 	IO::outb(0xA1, 0x28);
// 	IO::outb(0x21, 0x04);
// 	IO::outb(0xA1, 0x02);
// 	IO::outb(0x21, 0x01);
// 	IO::outb(0xA1, 0x01);
// 	IO::outb(0x21, 0x00);
// 	IO::outb(0xA1, 0x00);

// 	// Mask all interrupts except keyboard
// 	IO::outb(0x21, 0xFF & ~3);

// 	// Enable interrupts
// 	asm volatile ("sti");
// }




// void isr_set( uint8_t interrupt, void* handler )
// {
// 	isr[interrupt] = handler;
// }


// struct isr_frame
// {
// 	uint64_t cr0, cr2, cr3, cr4;
// 	uint64_t rdi, rsi, rbp;
// 	uint64_t rax, rbx, rcx, rdx;
// 	uint64_t vcode, ecode;
// 	uint64_t rip, cs, flags, rsp, ss;
// };


// extern "C"
// void __isr_dispatch( isr_frame *frame )
// {
//     SYSLOG_BEGIN("__interrupt_dispatch");
//     SYSLOG("vcode: %lu", frame->vcode);
//     SYSLOG("ecode: %lu", frame->ecode);
//     // SYSLOG("rdi: %lu", frame->rdi);
//     // SYSLOG("rsi: %lu", frame->rsi);
//     // SYSLOG("rdx: %lu", frame->rdx);
//     // SYSLOG("rcx: %lu", frame->rcx);
//     // SYSLOG("r8:  %lu", frame->r8);
//     // SYSLOG("r9:  %lu", frame->r9);

//     SYSLOG_END();

//     asm volatile ("cli; hlt");
// }



// // void isr_handler( isr_stack s)
// // {
// // 	if (s.interrupt >= 32)
// //     {
// // 		if (s.interrupt >= 40)
// //         {
// //             IO::outb(0xA0, 0x20);
// //         }

// //         IO::outb(0x20, 0x20);
// // 		void* handler = isr[s.interrupt];
// // 		if (handler)
// //         {
// //             ((void (*)(struct isr_stack)) handler)(s);
// //         }

// // 		else
// //         {
// //             printf("Unhandled interrupt 0x%x\n", s.interrupt);
// //         }

// // 	}

// //     else
// //     {
// // 		extern struct proc* current_proc;

// // 		if (current_proc->pid == 0)
// //         {
// //             panic_isr(s);
// //         }

// // 		else
// //         {
// //             panic_isr_user(s);
// //         }
// // 	}
// // }

