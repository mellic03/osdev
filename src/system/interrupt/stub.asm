section .text
    global interrupt_stub

interrupt_stub:
    push rax
    push rbx

    ; Push other registers here
    push r14
    push r15

    mov rsp, rdi
    call interrupt_dispatch
    mov rax, rsp

    pop r15
    pop r14
    ; Pop other registers here
    pop rbx
    pop rax

    ; Remove the vector number + error code
    add rsp, 16

    iret


