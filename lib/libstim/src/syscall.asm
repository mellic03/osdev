[bits 64]


section .text

    ; global stim_syscall
    ; stim_syscall:
    ;     mov rax, rdi
    ;     mov [rax], rsi
    ;     int 99
    ;     ret

    global stim_syscall
    stim_syscall:
        mov rax, rdi
        mov rbx, rsi
        int 99
        ret
