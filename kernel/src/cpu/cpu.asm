[bits 64]

global cpu_fxsave
global cpu_fxrstor

global cpu_set_rsp

global cpu_get_cr3
global cpu_set_cr3

global cpu_get_rflags
; global cpu_set_rflags

global cpu_enable_sse
global cpu_enable_avx

global GDT64_Install
; global cpu_load_gdt
; global cpu_flush_gdt




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
    cpu_set_rsp:
        mov rsp, rdi
        ret

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

    ; align 16
    ; cpu_load_gdt:
    ;     lgdt [rdi]
    ;     ret

    ; align 16
    ; cpu_flush_gdt:
    ;     ; Reload CS register:
    ;     push  0x08                  ; Push code segment to stack, 0x08 is a stand-in for your code segment
    ;     lea   rax, [rel .reload_cs] ; Load address of .reload_CS into rax
    ;     push  rax                   ; Push this value to the stack
    ;     retfq                       ; Perform a far return, RETFQ or LRETQ depending on syntax
    ; .reload_cs:
    ;     ; Reload data segment registers
    ;     mov   ax, 0x10 ; 0x10 is a stand-in for your data segment
    ;     mov   ds, ax
    ;     mov   es, ax
    ;     mov   fs, ax
    ;     mov   gs, ax
    ;     mov   ss, ax
    ;     ret
    

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
        mov ax,  0x10 ; 0x10 is a stand-in for your data segment
        mov ds,  ax
        mov es,  ax
        mov fs,  ax
        mov gs,  ax
        mov ss,  ax
        ret


    align 16
    global cpuid_proc_feat_info
    cpuid_proc_feat_info:
        push rbp
        mov rbp, rsp
        mov eax, 1
        cpuid
        mov dword [rdi], eax
        mov dword [rdi + 4], edx
        mov dword [rdi + 8], ecx
        mov dword [rdi + 12], ebx
        mov rsp, rbp
        pop rbp
        ret
