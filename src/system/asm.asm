; [bits 64]

; %macro __prologue__ 1
; 	push  rbp
; 	mov   rbp,   rsp
; 	sub   rsp,   %1
; %endmacro

; %macro __epilogue__ 1
; 	mov   rsp,   rbp
; 	pop   rbp
; 	ret
; %endmacro


; global __idk_asm_pushq
; __idk_asm_pushq:
;     __prologue__(8)
;     push rdi
;     __epilogue__(0)



; global __idk_asm_popq
; __idk_asm_popq:
;     __prologue__(16)
;     pop rax
;     mov rax, [rbp+8]
;     __epilogue__(0)