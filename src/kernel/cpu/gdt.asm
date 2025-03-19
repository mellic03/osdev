[bits 64]
   global __gdtr_limit
   global __gdtr_base


section .data
   align 8
   __gdtr_limit dw 0
   __gdtr_base  dq 0


section .text
   
   global __GDT_load
   __GDT_load:
      mov   di, [__gdtr_limit]
      mov   rsi, [__gdtr_limit+2]
      lgdt  [__gdtr_limit]
      ret



   global __GDT_flush
   __GDT_flush:
      ; Reload CS register:
      push  0x08                  ; Push code segment to stack, 0x08 is a stand-in for your code segment
      lea   rax, [rel .reload_CS] ; Load address of .reload_CS into rax
      push  rax                   ; Push this value to the stack
      retfq                       ; Perform a far return, RETFQ or LRETQ depending on syntax
   .reload_CS:
      ; Reload data segment registers
      mov   ax, 0x10 ; 0x10 is a stand-in for your data segment
      mov   ds, ax
      mov   es, ax
      mov   fs, ax
      mov   gs, ax
      mov   ss, ax
      ret