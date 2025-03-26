[bits 64]


section .text
    global __KThread_yield
    __KThread_yield:
        mov rax, rdi
        int 98
        ret


; KThread.rip    equ 0x00  ; Offset of rip
; KThread.rsp    equ 0x08  ; Offset of rsp
; KThread.rbx    equ 0x10  ; Offset of rbx
; KThread.rbp    equ 0x18  ; Offset of rbp
; KThread.r12    equ 0x20  ; Offset of r12
; KThread.r13    equ 0x28  ; Offset of r13
; KThread.r14    equ 0x30  ; Offset of r14
; KThread.r15    equ 0x38  ; Offset of r15
; KThread.next   equ 0x40  ; Offset of next (pointer to the next thread)



; %macro ctx_push 0
;     ; push rip
;     push rsp
;     push rbx
;     push rbp
;     push r12
;     push r13
;     push r14
;     push r15
; %endmacro



; %macro ctx_pop 0
;     pop r15
;     pop r14
;     pop r12
;     pop r13
;     pop rbp
;     pop rbx
;     pop rsp
;     ; pop rip
; %endmacro




; section .text

;     global __kthread_yield
;     __kthread_yield:
;         ctx_push
;         mov rsp, [rdi + 1*8]
;         ctx_pop
;         ret



;     global __kthread_switch
;     __kthread_switch:
;         push rax
;         push rcx
;         push rdx
;         push rbx
;         push rbp
;         push rsi
;         push rdi
;         push r8
;         push r9
;         push r10
;         push r11
;         push r12
;         push r13
;         push r14
;         push r15
        
;         ; Save current rsp (current task's stack pointer) to the task struct
;         mov r15, [rdi]            ; Load task_struct from rdi into r15
;         mov [r15], rsp            ; Store current rsp value into task_struct->rsp_val

;         ; Save the second argument (next task's struct) into the current task's struct
;         mov [rdi], rsi            ; Store the next task's struct address into task_struct->next_task_struct
;         mov rsp, [rsi]            ; Switch rsp to the next task's stack (from next task's struct)

;         ; At this point, control has switched to the next task's stack

;         ; Restore registers from stack (after switching control back to current task)
;         pop rdi
;         pop r15
;         pop r14
;         pop r13
;         pop r12
;         pop r11
;         pop r10
;         pop r9
;         pop r8
;         pop rdi
;         pop rsi
;         pop rbp
;         pop rbx
;         pop rdx
;         pop rcx
;         pop rax

;         ; Return control back to the caller
;         ret