[bits 64]



; struct kTCB
; {
;     uint64_t tid;
;     uint64_t rsp;
;     uint64_t rip;
;     uint64_t status;

;     struct {
;         uint64_t r11, r12, r13, r14, r15;
;         uint64_t rax, rbx, rcx, rdx;
;         uint64_t rdi, rsi, rbp;
;     } registers;

;     kTCB *next;

; };

; extern __kTCB_curr

section .data
    align 16
    global test_jump_retvalue
    test_jump_retvalue dq 0

; https://wiki.osdev.org/System_V_ABI#x86-64
; Functions are called using the call instruction, which pushes the
; address of the next instruction onto the stack and jumps to the operand.
; Functions return to the caller using the ret instruction, which pops
; the return address from the stack and jumps to it.
; The stack is 16-byte aligned just before the call instruction is executed. 

section .text
    align 16

    test_fn_call:
        ; do stuff
        ret

    test_main_call:
        call test_fn_call
        ; done
        ret

    test_fn_jump:
        ; do stuff
        push rax
        mov  rax, 1234
        mov  [test_jump_retvalue], rax
        pop  rax

        ; return
        add  rsp, 8
        jmp  [rsp - 8]

    global test_main_jump
    test_main_jump:
        push rbp        ; save prev rpb on stack
        mov  rbp, rsp   ; save prev rsp in rbp
        and  rsp, 0xFFFFFFFFFFFFFFF0

        push .return_address
        jmp  test_fn_jump

        .return_address:
        mov  rsp, rbp   ; restore prev rsp
        pop  rbp        ; restore prev rbp

        ; return
        ret



; extern kthread_start
; extern kthread_switch

; section .text
;     %macro ctx_push 0
;         push rbp
;         push rsi
;         push rdi
;         push rdx
;         push rcx
;         push rbx
;         push rax
;         push r15
;         push r14
;         push r13
;         push r12
;         push r11
;         push rsp
;     %endmacro

;     %macro ctx_pop 0
;         pop rsp
;         pop r11
;         pop r12
;         pop r13
;         pop r14
;         pop r15
;         pop rax
;         pop rbx
;         pop rcx
;         pop rdx
;         pop rdi
;         pop rsi
;         pop rbp
;     %endmacro


;     global __kthread_start
;     __kthread_start:
;         sub rsp, 8
;         ctx_push

;         sub rsp, 8
;         mov rdi, rsp
;         add rsp, 8
;         call kthread_start

;         ctx_pop
;         add rsp, 8
;         ret


;     global __kthread_yield
;     __kthread_yield:
;         sub rsp, 8
;         ctx_push

;         sub rsp, 8
;         mov rdi, rsp
;         add rsp, 8
;         call kthread_switch

;         ctx_pop
;         add rsp, 8
;         ret

