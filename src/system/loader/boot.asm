; https://wiki.osdev.org/Bare_Bones_with_NASM


%define MB2_SEARCH 32768
%define MB2_HEADER_ALIGN 8

%define MB2_HEADER_MAGIC 0xe85250d6

%define MB2_BOOTLOADER_MAGIC 0x36d76289

%define MB2_MOD_ALIGN 0x00001000
%define MB2_INFO_ALIGN 0x00000008

%define MB2_TAG_ALIGN 8
%define MB2_TAG_TYPE_END 0
%define MB2_TAG_TYPE_CMDLINE 1
%define MB2_TAG_TYPE_BOOT_LOADER_NAME 2
%define MB2_TAG_TYPE_MODULE 3
%define MB2_TAG_TYPE_BASIC_MEMINFO 4
%define MB2_TAG_TYPE_BOOTDEV 5
%define MB2_TAG_TYPE_MMAP 6
%define MB2_TAG_TYPE_VBE 7
%define MB2_TAG_TYPE_FRAMEBUFFER 8
%define MB2_TAG_TYPE_ELF_SECTIONS 9
%define MB2_TAG_TYPE_APM 10
%define MB2_TAG_TYPE_EFI32 11
%define MB2_TAG_TYPE_EFI64 12
%define MB2_TAG_TYPE_SMBIOS 13
%define MB2_TAG_TYPE_ACPI_OLD 14
%define MB2_TAG_TYPE_ACPI_NEW 15
%define MB2_TAG_TYPE_NETWORK 16
%define MB2_TAG_TYPE_EFI_MMAP 17
%define MB2_TAG_TYPE_EFI_BS 18
%define MB2_TAG_TYPE_EFI32_IH 19
%define MB2_TAG_TYPE_EFI64_IH 20
%define MB2_TAG_TYPE_LOAD_BASE_ADDR 21

%define MB2_HEADER_TAG_END 0
%define MB2_HEADER_TAG_INFORMATION_REQUEST 1
%define MB2_HEADER_TAG_ADDRESS 2
%define MB2_HEADER_TAG_ENTRY_ADDRESS 3
%define MB2_HEADER_TAG_CONSOLE_FLAGS 4
%define MB2_HEADER_TAG_FRAMEBUFFER 5
%define MB2_HEADER_TAG_MODULE_ALIGN 6
%define MB2_HEADER_TAG_EFI_BS 7
%define MB2_HEADER_TAG_ENTRY_ADDRESS_EFI32 8
%define MB2_HEADER_TAG_ENTRY_ADDRESS_EFI64 9
%define MB2_HEADER_TAG_RELOCATABLE 10

%define MB2_ARCHITECTURE_I386 0
%define MB2_ARCHITECTURE_MIPS32 4
%define MB2_HEADER_TAG_OPTIONAL 1

%define MB2_LOAD_PREFERENCE_NONE 0
%define MB2_LOAD_PREFERENCE_LOW 1
%define MB2_LOAD_PREFERENCE_HIGH 2

%define MB2_CONSOLE_FLAGS_CONSOLE_REQUIRED 1
%define MB2_CONSOLE_FLAGS_EGA_TEXT_SUPPORTED 2


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
global __multiboot

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





global vga_thing
vga_thing:
        ; Video mode
        mov ah, 00h
        mov al, 03h
        INT 10h

        mov ax, 1100h
        mov bl, 0
        INT 10h
.end:



; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
; Declare _start as a function symbol with the given symbol size.

section .text
global _start:function (_start.end - _start)
_start:
	; Set stack pointer
	mov esp, stack_top

	; Pass parameters to lmain
	push ebx
	push eax

	extern lmain
	call lmain

        ; extern _init
        ; call _init

	; If the system has nothing more to do, put the computer into an
	; infinite loop. To do that:
	; 1) Disable interrupts with cli (clear interrupt enable in eflags).
	;    They are already disabled by the bootloader, so this is not needed.
	;    Mind that you might later enable interrupts and return from
	;    lmain (which is sort of nonsensical to do).
	; 2) Wait for the next interrupt to arrive with hlt (halt instruction).
	;    Since they are disabled, this will lock up the computer.
	; 3) Jump to the hlt instruction if it ever wakes up due to a
	;    non-maskable interrupt occurring or due to system management mode.
	cli
.hang:	hlt
	jmp .hang

.end:



global _nasm_gdt_load
_nasm_gdt_load:
    gdtr dd 0 ; For limit storage
    dd 0      ; For base storage

    mov   ax, [esp+4]
    mov  [gdtr], AX
    mov   eax, [esp+8]
    mov  [gdtr + 2], eax
    LGDT [gdtr]
    ret

.end:






