[bits 64]


section .text

global __cpu_enable_SSE
__cpu_enable_SSE:
    mov rax, cr0
    and  ax, 0xFFFB
    or   ax, 0x2
    mov cr0, rax
    mov rax, cr4
    or   ax, 3 << 9
    mov cr4, rax
    ret




get_rip:
    mov rax, [rsp]
    ret


%macro ctx_save 0
    push rbp
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax
    push r15
    push r14
    push r13
    push r12
    push r11
    call get_rip ;
    push rax     ; Instruction pointer
%endmacro


%macro ctx_load 0
    call get_rip ;
    pop  rax     ; Instruction pointer
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
    pop rbp
%endmacro



%macro ctx_save 1
    mov [$1 + 0*8],  r11
    mov [$1 + 1*8],  r12
    mov [$1 + 2*8],  r13
    mov [$1 + 3*8],  r14
    mov [$1 + 4*8],  r15
    mov [$1 + 5*8],  rax
    mov [$1 + 6*8],  rbx
    mov [$1 + 7*8],  rcx
    mov [$1 + 8*8],  rdx
    mov [$1 + 9*8],  rdi
    mov [$1 + 10*8], rsi
    mov [$1 + 11*8], rbp
    mov [$1 + 12*8], rbp  ; ecode
    mov [$1 + 13*8], rbp  ; vcode
    call get_rip
    mov [$1 + 14*8], rax  ; Instruction pointer

%endmacro


%macro ctx_load 1
    mov r11, [$1 + 0*8]
    mov r12, [$1 + 1*8]
    mov r13, [$1 + 2*8]
    mov r14, [$1 + 3*8]
    mov r15, [$1 + 4*8]
    mov rax, [$1 + 5*8]
    mov rbx, [$1 + 6*8]
    mov rcx, [$1 + 7*8]
    mov rdx, [$1 + 8*8]
    mov rdi, [$1 + 9*8]
    mov rsi, [$1 + 10*8]
    mov rbp, [$1 + 11*8]
    mov rbp, [$1 + 12*8]  ; ecode
    mov rbp, [$1 + 13*8]  ; vcode
    ; jmp [$1 + 14*8]       ; Instruction pointer
%endmacro




global cpu_ctx_load
cpu_ctx_load:
    ctx_save rdi
    ret

    

global cpu_ctx_save
cpu_ctx_save:
    ctx_save rdi
    ret

