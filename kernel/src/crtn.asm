[bits 64]

section .init
    align 16
	; gcc will nicely put the contents of crtend.o's .init section here.
	pop qword rbp
	ret

section .fini
    align 16
	; gcc will nicely put the contents of crtend.o's .fini section here.
	pop qword rbp
	ret