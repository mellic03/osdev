
global __enable_SSE
__enable_SSE:
    mov rax, cr0
    and  ax, 0xFFFB
    or   ax, 0x2
    mov cr0, rax
    mov rax, cr4
    or   ax, 3 << 9
    mov cr4, rax
    ret
