[bits 64]

section .data
    align 8
    global context_arg
    extern context_arg2
    context_arg dq 12*8
    context_arg2 dq 12*8



extern kthread_switch
extern __kthread_start
extern __kthread_yield

%macro bitch 1
    mov rax, [rsp-%1*8]
    mov [context_arg2 + %1*8], rax
%endmacro


section .text
    %macro ctx_push 0
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
    %endmacro

    %macro ctx_pop 0
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


    global kthread_start
    kthread_start:
        ctx_push
        mov rdi, rsp
        sub rdi, 16
        call kthread_switch
        ctx_pop
        ret

    global kthread_yield
    kthread_yield:
        ctx_push
        mov rdi, rsp
        sub rdi, 16
        call __kthread_yield
        ctx_pop
        ret

