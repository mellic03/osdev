[bits 64]

; global cpu_fxsave
; global cpu_fxrstor

global cpu_get_cr3
global cpu_set_cr3

; global cpu_read_rflags
; global cpu_write_rflags

global cpu_enable_sse
global cpu_enable_avx

global cpu_load_gdt
global cpu_flush_gdt




section .text

    ; align 16
    ; cpu_fxsave:
    ;     fxsave [rdi]
    ;     ret

    ; align 16
    ; cpu_fxrstor:
    ;     fxrstor [rdi]
    ;     ret


    align 16
    cpu_get_cr3:        ; uint64_t cpu_get_cr3( void )
        mov rax, cr3
        ret

    align 16
    cpu_set_cr3:       ; void cpu_set_cr3( uint64_t )
        mov cr3, rdi
        ret


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
    cpu_enable_avx:
        push rax
        push rcx
        push rdx

        xor  rcx, rcx
        xgetbv          ; load XCR0 register
        or eax, 7       ; set AVX, SSE, X87 bits
        xsetbv          ; save back to XCR0

        pop rdx
        pop rcx
        pop rax
        ret


    align 16
    cpu_load_gdt:           ; void cpu_load_gdt( void* )
        lgdt [rdi]
        ret


    align 16
    cpu_flush_gdt:          ; void cpu_flush_gdt( void )
        ; Reload CS register:
        push  0x08                  ; Push code segment to stack, 0x08 is a stand-in for your code segment
        lea   rax, [rel .reload_cs] ; Load address of .reload_CS into rax
        push  rax                   ; Push this value to the stack
        retfq                       ; Perform a far return, RETFQ or LRETQ depending on syntax
    .reload_cs:
        ; Reload data segment registers
        mov   ax, 0x10 ; 0x10 is a stand-in for your data segment
        mov   ds, ax
        mov   es, ax
        mov   fs, ax
        mov   gs, ax
        mov   ss, ax
        ret