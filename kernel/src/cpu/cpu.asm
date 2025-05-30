[bits 64]

global cpu_fxsave
global cpu_fxrstor

global CPU_GetCR3
global CPU_SetCR3

global cpu_get_rflags


global GDT64_Install


section .data
    align 16
    fcw: dw 0x037F
    mem: dd 0
    mxcsr_mask: dd 0
    align 16
    fxsave_region: times 512 db 0




section .text
    align 16

    global cpu_enable_fpu
    cpu_enable_fpu:
        mov rax, cr0
        bts rax, 1		; Set Monitor co-processor (Bit 1)
        btr rax, 2		; Clear Emulation (Bit 2)
        bts rax, 5		; Set Native Exception (Bit 5)
        btr rax, 3		; Clear TS
        mov cr0, rax
        finit
        ret

    global cpu_enable_sse
    cpu_enable_sse:
        mov rax, cr4
        bts rax, 9		; Set Operating System Support for FXSAVE and FXSTOR instructions (Bit 9)
        bts rax, 10		; Set Operating System Support for Unmasked SIMD Floating-Point Exceptions (Bit 10)
        mov cr4, rax
        ret

    global cpu_enable_xsave
    cpu_enable_xsave:
        mov rax, cr4
        bts rax, 18		; Set Operating System Support for XSave (Bit 18)
        mov cr4, rax
        
        ; Set the control register
        mov rcx, 0
        xgetbv
        or  eax, 6
        mov rcx, 0
        xsetbv
        ret

    global cpu_enable_avx
    cpu_enable_avx:
        mov rax, cr4
        bts rax, 14		; Set Operating System Support for Advanced Vector Extensions (Bit 14)
        mov cr4, rax
        ret 


    ; https://www.felixcloutier.com/x86/stos:stosb:stosw:stosd:stosq
    ; The STOSX instructions can be preceded by the REP prefix
    ; for block stores of ECX bytes, words, or doublewords.

    global cpu_STOSD
    cpu_STOSD:          ; void cpu_STOSD( void *dst, uint32_t src, size_t count )
        mov rax, rsi
        mov rcx, rdx
        rep stosd ; stosq stores rax at rdi
        ret


    cpu_fxsave:
        fxsave64 [rdi]
        ret

    cpu_fxrstor:
        fxrstor64 [rdi]
        ret

    CPU_GetCR3:        ; uint64_t CPU_GetCR3( void )
        mov rax, cr3
        ret

    CPU_SetCR3:       ; void CPU_SetCR3( uint64_t )
        mov cr3, rdi
        ret



    global GDT64_Install
    GDT64_Install:
        lgdt [rdi]
        mov rbp, rsp
        push 0x10
        push rbp
        pushf
        push 0x8
        push .cont
        iretq
    .cont:
        mov ax, 0x10 ; 0x10 is a stand-in for your data segment
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        ret
