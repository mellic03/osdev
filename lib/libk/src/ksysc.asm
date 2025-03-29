[bits 64]

section .text
    global libk_syscall
    libk_syscall:
        int 99
        ret

