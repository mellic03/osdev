; https://wiki.osdev.org/Bare_Bones_with_NASM


%include "../src/kernel/boot/multiboot2.asm"


; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:



extern _edata
extern _ebss

%define MB2_ARCH  MB2_ARCHITECTURE_I386
%define MB2_LEN   (mbend-mbhead)


section .multiboot
align 8
multiboot_header:
        dd MB2_HEADER_MAGIC
        dd MB2_ARCH
        dd multiboot_header_end - multiboot_header
        dd -(MB2_HEADER_MAGIC + MB2_ARCH + (multiboot_header_end - multiboot_header))

mb2_tag_info_start:
        dw MB2_HEADER_TAG_INFORMATION_REQUEST
        dw 0
        dd mb2_tag_info_end - mb2_tag_info_start
        dd 1
        dd 2
        dd 6
mb2_tag_info_end:

align 8
mb2_tag_console_start:
        dw MB2_HEADER_TAG_CONSOLE_FLAGS
        dw 0
        dd mb2_tag_console_end - mb2_tag_console_start
        dd 0x3
mb2_tag_console_end:


; align 8
; mb2_tag_mmap_start:
;         dw MB2_TAG_TYPE_MMAP
;         dw 0
;         dd mb2_tag_console_end - mb2_tag_console_start
;         dd 0x3
; mb2_tag_mmap_end:



align 8
mb2_tag_fb_start:
        dw 0; MB2_HEADER_TAG_FRAMEBUFFER
        dw 1
        dd mb2_tag_fb_end - mb2_tag_fb_start
        dd 1920
        dd 1080
        dd 32
mb2_tag_fb_end:


align 8
mb2_tag_end_start:
        dw 0
        dw 0
        dd mb2_tag_end_end - mb2_tag_end_start
mb2_tag_end_end:

multiboot_header_end:








; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
; Declare _start as a function symbol with the given symbol size.

section .text
global _start:function (_start.end - _start)
_start:
	; Set stack pointer
	mov esp, stack_top

	; Reset EFLAGS
	push dword 0
	popf

	; Pass parameters to kmain
	push ebx
	push eax

	extern kmain
	call kmain

	; If the system has nothing more to do, put the computer into an
	; infinite loop. To do that:
	; 1) Disable interrupts with cli (clear interrupt enable in eflags).
	;    They are already disabled by the bootloader, so this is not needed.
	;    Mind that you might later enable interrupts and return from
	;    kmain (which is sort of nonsensical to do).
	; 2) Wait for the next interrupt to arrive with hlt (halt instruction).
	;    Since they are disabled, this will lock up the computer.
	; 3) Jump to the hlt instruction if it ever wakes up due to a
	;    non-maskable interrupt occurring or due to system management mode.
	cli
.hang:	hlt
	jmp .hang

.end: