[bits 64]

global cpu_fxsave
global cpu_fxrstor

global CPU_GetCR3
global CPU_SetCR3

global cpu_get_rflags

global cpu_enable_sse
global cpu_enable_avx

global GDT64_Install


section .text

    align 16
    cpu_fxsave:
        fxsave64 [rdi]
        ret

    align 16
    cpu_fxrstor:
        fxrstor64 [rdi]
        ret

    align 16
    CPU_GetCR3:        ; uint64_t CPU_GetCR3( void )
        mov rax, cr3
        ret

    align 16
    CPU_SetCR3:       ; void CPU_SetCR3( uint64_t )
        mov cr3, rdi
        ret


    ; align 16
    ; cpu_enable_sse:
    ;     mov rax, cr0
    ;     bts rax, 1		; Set Monitor co-processor (Bit 1)
    ;     btr rax, 2		; Clear Emulation (Bit 2)
    ;     bts rax, 5		; Set Native Exception (Bit 5)
    ;     btr rax, 3		; Clear TS
    ;     mov cr0, rax
    ;     finit
    ;     mov rax, cr4
    ;     bts rax, 9		; Set Operating System Support for FXSAVE and FXSTOR instructions (Bit 9)
    ;     bts rax, 10		; Set Operating System Support for Unmasked SIMD Floating-Point Exceptions (Bit 10)
    ;     mov cr4, rax
    ;     ret 



    align 16
    cpu_enable_sse:
        mov rax, cr0
        and  ax, 0xFFFB
        or   ax, 0x2
        mov cr0, rax
        mov rax, cr4
        or   ax, 3 << 9
        mov cr4, rax
        ret


    align 16
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
