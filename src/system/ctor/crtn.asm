; x86_64 crtn.asm

section .init
	; gcc will nicely put the contents of crtend.o's .init section here.
	pop qword rbp
	ret

section .fini
	; gcc will nicely put the contents of crtend.o's .fini section here.
	pop qword rbp
	ret