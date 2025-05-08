; ; [bits 64]

; ; global GDT64_ptr
; ; global GDT64_load


; ; section .data
; ;     align 16
; ;     GDT64:                           ; Global Descriptor Table (64-bit).
; ;         .Null: equ $ - GDT64         ; The null descriptor.
; ;         dw 0xFFFF                    ; Limit (low).
; ;         dw 0                         ; Base (low).
; ;         db 0                         ; Base (middle)
; ;         db 0                         ; Access.
; ;         db 0                         ; Granularity.
; ;         db 0                         ; Base (high).
; ;         .Code: equ $ - GDT64         ; The code descriptor.
; ;         dw 0                         ; Limit (low).
; ;         dw 0                         ; Base (low).
; ;         db 0                         ; Base (middle)
; ;         db 10011010b                 ; Access (exec/read).
; ;         db 00100000b                 ; Granularity, 64 bits flag, limit19:16.
; ;         db 0                         ; Base (high).
; ;         .Data: equ $ - GDT64         ; The data descriptor.
; ;         dw 0                         ; Limit (low).
; ;         dw 0                         ; Base (low).
; ;         db 0                         ; Base (middle)
; ;         db 10010010b                 ; Access (read/write).
; ;         db 00000000b                 ; Granularity.
; ;         db 0                         ; Base (high).
; ;         .UserData: equ $ - GDT64     ; The usermode data descriptor.
; ;         dw 0                         ; Limit (low).
; ;         dw 0                         ; Base (low).
; ;         db 0                         ; Base (middle)
; ;         db 11110010b                 ; Access (read/write).
; ;         db 00000000b                 ; Granularity.
; ;         db 0                         ; Base (high).
; ;         .UserCode: equ $ - GDT64     ; The usermode code descriptor.
; ;         dw 0                         ; Limit (low).
; ;         dw 0                         ; Base (low).
; ;         db 0                         ; Base (middle)
; ;         db 11111010b                 ; Access (exec/read).
; ;         db 00100000b                 ; Granularity, 64 bits flag, limit19:16.
; ;         db 0                         ; Base (high).
; ;         .TSS: ;equ $ - GDT64         ; TSS Descriptor
; ;         .len:
; ;         dw 108                       ; TSS Length - the x86_64 TSS is 108 bytes loong
; ;         .low:
; ;         dw 0                         ; Base (low).
; ;         .mid:
; ;         db 0                         ; Base (middle)
; ;         db 10001001b                 ; Flags
; ;         db 00000000b                 ; Flags 2
; ;         .high:
; ;         db 0                         ; Base (high).
; ;         .high32:
; ;         dd 0                         ; High 32 bits
; ;         dd 0                         ; Reserved
; ;     GDT64_ptr:                       ; The GDT-pointer.
; ;         dw $ - GDT64 - 1             ; Limit.
; ;         dq GDT64                     ; Base.




; section .text
;     align 16
; ;     GDT64_load:
; ;         lgdt [GDT64_ptr]

; ;         mov rbp, rsp
; ;         push 0x10
; ;         push rbp
; ;         pushf
; ;         push 0x8
; ;         push .cont
; ;         iretq

; ;     .cont:
; ;         mov ax,  0x10 ; 0x10 is a stand-in for your data segment
; ;         mov ds,  ax
; ;         mov es,  ax
; ;         mov fs,  ax
; ;         mov gs,  ax
; ;         mov ss,  ax
; ;         ret


