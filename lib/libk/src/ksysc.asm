[bits 64]

section .text
    global __libk_syscall
    __libk_syscall:
        int 99
        ret

