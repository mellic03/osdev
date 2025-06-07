[bits 64]

section .init
    align 16
    global _init
    _init:
        push rbp
        mov rbp, qword rsp
        ; gcc will nicely put the contents of crtbegin.o's .init section here.

section .fini
    align 16
    global _fini
    _fini:
        push rbp
        mov rbp, qword rsp
        ; gcc will nicely put the contents of crtbegin.o's .fini section here.
