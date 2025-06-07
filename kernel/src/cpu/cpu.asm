[bits 64]

; Parameters to functions are passed in via the registers
; rdi, rsi, rdx, rcx, r8, and r9. Floating-point parameters
; are passed in via xmm0 through xmm7.

section .data
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
        mov rax, cr0
        and ax,  0xFFFB		; clear coprocessor emulation CR0.EM
        or  ax,  0x2		; set coprocessor monitoring  CR0.MP
        mov cr0, rax
        mov rax, cr4
        or  ax,  3 << 9		; set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
        mov cr4, rax
        ret

    global cpu_enable_avx
    cpu_enable_avx:
        mov rax, cr4
        or  eax, 0x40000
        mov cr4, rax

        mov rcx, 0
        xgetbv

        or eax, 6
        mov rcx, 0
        xsetbv
        ret

    global cpu_enable_xsave
    cpu_enable_xsave:
        mov rax, cr4
        or  rax, 1 << 18
        mov cr4, rax 
        ret


    global cpu_fxsave64
    cpu_fxsave64:
        fxsave64 [rdi]
        ret

    global cpu_fxrstor64
    cpu_fxrstor64:
        fxrstor64 [rdi]
        ret


    ; https://www.felixcloutier.com/x86/stos:stosb:stosw:stosd:stosq
    ; The STOSX instructions can be preceded by the REP prefix
    ; for block stores of ECX bytes, words, or doublewords.

    global cpu_stos8
    cpu_stos8:           ; void cpu_stos8( void *dst, uint32_t src, size_t count )
        mov rax, rsi
        mov rcx, rdx
        rep stosb       ; stosx stores rax at rdi
        ret

    global cpu_stos32
    cpu_stos32:          ; void cpu_stos32( void *dst, uint32_t src, size_t count )
        mov rax, rsi
        mov rcx, rdx
        rep stosd       ; stosq stores rax at rdi
        ret

    global cpu_stos64
    cpu_stos64:
        mov rax, rsi
        mov rcx, rdx
        rep stosq
        ret


    global cpu_movs8    ; void cpu_movs8( rdi, rsi, rdx )
    cpu_movs8:          ; void cpu_movs8( void *dst, const void *src, size_t count )
        mov rcx, rdx
        rep movsb       ; Move byte from address RSI to RDI
        ret             ; rep repeats action RCX times.

    global cpu_movs32
    cpu_movs32:
        mov rcx, rdx
        rep movsd
        ret

    global cpu_movs64
    cpu_movs64:
        mov rcx, rdx
        rep movsq
        ret


    global cpu_getCR3
    cpu_getCR3:        ; uint64_t cpu_getCR3( void )
        mov rax, cr3
        ret

    global cpu_setCR3
    cpu_setCR3:       ; void cpu_setCR3( uint64_t )
        mov cr3, rdi
        ret

    global cpu_getTSC
    cpu_getTSC:
        rdtsc       ; read time-stamp counter into rdx:rax (hi:lo).
        mov [rdi], dword edx
        mov [rsi], dword eax
        ; shr rax, 32
        ; or  rax, rdx
        ret


    global cpu_installGDT
    cpu_installGDT:
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

