[bits 64]

section .text
    global libc_syscall
    libc_syscall:
        int 99
        ret

