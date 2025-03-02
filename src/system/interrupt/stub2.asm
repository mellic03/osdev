section .text
    global vector_0_handler, vector_1_handler, vector_13_handler

; Align to a 16-byte boundary
align 16
vector_0_handler:
    ; Vector 0 has no error code
    pushq 0
    ; Push the vector number
    pushq 0
    jmp interrupt_stub

; Align to the next 16-byte boundary
align 16
vector_1_handler:
    ; Vector 1 also needs a dummy error code
    pushq 0
    ; Push the vector number
    pushq 1
    jmp interrupt_stub

; Skipping ahead

; Align to the next 16-byte boundary
align 16
vector_13_handler:
    ; Vector 13 (#GP) does push an error code, so we won't. Just the vector number.
    pushq 13
    jmp interrupt_stub
