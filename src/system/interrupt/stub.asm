[bits 64]

section .text

%define NUM_INTERRUPTS 128
extern __isr_dispatcher

; global divideException
; global debugException
; global breakpointException
; global overflowException
; global boundRangeExceededException
; global invalidOpcodeException
; global deviceNotAvaliableException
; global doubleFaultException ; no push 0
; global coprocessorSegmentOverrunException
; global invalidTSSException ; no push 0
; global segmentNotPresentException ; no push 0
; global stackSegmentFaultException ; no push 0
; global generalProtectionFaultException ; no push 0
; global pageFaultException ; no push 0
; ; skip one
; global floatingPointException
; global alignmentCheckException ; no push 0
; global machineCheckException
; global simdFloatingPointException
; global virtualisationException

; global syscallInterrupt



; divideException:
;     push 0
;     push 0
;     jmp baseHandler

; debugException:
;     push 0
;     push 1
;     jmp baseHandler

; breakpointException:
;     push 0
;     push 3
;     jmp baseHandler

; overflowException:
;     push 0
;     push 4
;     jmp baseHandler

; boundRangeExceededException:
;     push 0
;     push 5
;     jmp baseHandler

; invalidOpcodeException:
;     push 0
;     push 6
;     jmp baseHandler

; deviceNotAvaliableException:
;     push 0
;     push 7
;     jmp baseHandler

; doubleFaultException:
;     push 8
;     jmp baseHandler

; coprocessorSegmentOverrunException:
;     push 0
;     push 9
;     jmp baseHandler

; invalidTSSException:
;     push 10
;     jmp baseHandler

; segmentNotPresentException:
;     push 11
;     jmp baseHandler

; stackSegmentFaultException:
;     push 12
;     jmp baseHandler

; generalProtectionFaultException:
;     push 13
;     jmp baseHandler

; align 0x08, db 0x00
; pageFaultException:
;     push 14
;     jmp baseHandler

; floatingPointException:
;     push 0
;     push 16
;     jmp baseHandler

; alignmentCheckException:
;     push 17
;     jmp baseHandler

; machineCheckException:
;     push 0
;     push 18
;     jmp baseHandler

; simdFloatingPointException:
;     push 0
;     push 19
;     jmp baseHandler

; virtualisationException:
;     push 0
;     push 20
;     jmp baseHandler

; syscallInterrupt:
;     push 0
;     push 20
;     jmp baseHandler

; align 0x08, db 0x00
; baseHandler:
;    push rax
;    push rbx
;    push rcx
;    push rdx
;    push rsi
;    push rdi
;    push rbp
;    push r8
;    push r9
;    push r10
;    push r11
;    push r12
;    push r13
;    push r14
;    push r15
;    mov rax, cr2
;    push rax
;    cld
;    mov rdi, rsp
;    call __isr_dispatcher 
;    add rsp, 8
;    pop r15
;    pop r14
;    pop r13
;    pop r12
;    pop r11
;    pop r10
;    pop r9
;    pop r8
;    pop rbp
;    pop rdi
;    pop rsi
;    pop rdx
;    pop rcx
;    pop rbx
;    pop rax
;    add rsp, 0x10
;    iretq





%macro _err_stub 1
isr_stub_%+%1:
    ; push %1 ; Interrupt code
    mov rdi, qword %1
    call __isr_dispatcher
    ; pop rax
    ; pop rax
    iretq
%endmacro

%macro _noerr_stub 1
isr_stub_%+%1:
    ; push 0  ; Dummy error code
    ; push %1 ; Interrupt code
    mov rdi, qword %1
    call __isr_dispatcher
    ; pop rax
    ; pop rax
    iretq
%endmacro




_noerr_stub 0
_noerr_stub 1
_noerr_stub 2
_noerr_stub 3
_noerr_stub 4
_noerr_stub 5
_noerr_stub 6
_noerr_stub 7
_err_stub   8
_noerr_stub 9
_err_stub   10
_err_stub   11
_err_stub   12
_err_stub   13
_err_stub   14
_noerr_stub 15
_noerr_stub 16
_err_stub   17
_noerr_stub 18
_noerr_stub 19
_noerr_stub 20
_noerr_stub 21
_noerr_stub 22
_noerr_stub 23
_noerr_stub 24
_noerr_stub 25
_noerr_stub 26
_noerr_stub 27
_noerr_stub 28
_noerr_stub 29
_err_stub   30
_noerr_stub 31

_noerr_stub 32
_noerr_stub 33
_noerr_stub 34
_noerr_stub 35
_noerr_stub 36
_noerr_stub 37
_noerr_stub 38
_noerr_stub 39
_noerr_stub 40
_noerr_stub 41
_noerr_stub 42
_noerr_stub 43
_noerr_stub 44
_noerr_stub 45
_noerr_stub 46
_noerr_stub 47
_noerr_stub 48
_noerr_stub 49
_noerr_stub 50
_noerr_stub 51
_noerr_stub 52
_noerr_stub 53
_noerr_stub 54
_noerr_stub 55
_noerr_stub 56
_noerr_stub 57
_noerr_stub 58
_noerr_stub 59
_noerr_stub 60
_noerr_stub 61
_noerr_stub 62
_noerr_stub 63
_noerr_stub 64
_noerr_stub 65
_noerr_stub 66
_noerr_stub 67
_noerr_stub 68
_noerr_stub 69
_noerr_stub 70
_noerr_stub 71
_noerr_stub 72
_noerr_stub 73
_noerr_stub 74
_noerr_stub 75
_noerr_stub 76
_noerr_stub 77
_noerr_stub 78
_noerr_stub 79
_noerr_stub 80
_noerr_stub 81
_noerr_stub 82
_noerr_stub 83
_noerr_stub 84
_noerr_stub 85
_noerr_stub 86
_noerr_stub 87
_noerr_stub 88
_noerr_stub 89
_noerr_stub 90
_noerr_stub 91
_noerr_stub 92
_noerr_stub 93
_noerr_stub 94
_noerr_stub 95
_noerr_stub 96
_noerr_stub 97
_noerr_stub 98
_noerr_stub 99
_noerr_stub 100
_noerr_stub 101
_noerr_stub 102
_noerr_stub 103
_noerr_stub 104
_noerr_stub 105
_noerr_stub 106
_noerr_stub 107
_noerr_stub 108
_noerr_stub 109
_noerr_stub 110
_noerr_stub 111
_noerr_stub 112
_noerr_stub 113
_noerr_stub 114
_noerr_stub 115
_noerr_stub 116
_noerr_stub 117
_noerr_stub 118
_noerr_stub 119
_noerr_stub 120
_noerr_stub 121
_noerr_stub 122
_noerr_stub 123
_noerr_stub 124
_noerr_stub 125
_noerr_stub 126
_noerr_stub 127




global __isr_table
__isr_table:
%assign i 0 
%rep    128 
    dq isr_stub_%+i
%assign i i+1 
%endrep
