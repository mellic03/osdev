

; section .text
;     global _nasm_inb
;     global _nasm_outb


; _nasm_inb:
;     mov dx, [esp+4] ; port
;     inb al, dx
;     mov ax, al
;     ret

; ; global _nasm_outb
; ; _nasm_outb:
; ;     mov eax, [esp+4] ; port
; ;     mov dx, ax
; ;     mov al,  [esp+8] ; value
; ;     ret