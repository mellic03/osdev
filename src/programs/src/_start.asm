[bits 64]

global _start
extern libc_init
extern main

section .text
    _start:
        call libc_init
        call main
        ret

