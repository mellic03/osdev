[bits 64]

global _start
extern libc_init
extern main

section .text

_start:
    ; init user-mode libc/libc++
    call libc_init
    call main
    ; fini user-mode libc/libc++
    ret

