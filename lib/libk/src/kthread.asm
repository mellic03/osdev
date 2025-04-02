; [bits 64]

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

