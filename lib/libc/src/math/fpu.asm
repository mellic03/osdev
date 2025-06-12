[bits 64]

global FPU_S0
global FPU_S1
global FPU_R0
global FPU_R1

section .data
    align 16
    FPU_S0: dd 1.0
    FPU_S1: dd 1.0
    FPU_R0: dd 1.0
    FPU_R1: dd 1.0


section .text
    align 16

    global FPU_fsin
    FPU_fsin:
        fld  dword [FPU_S0]
        fsin
        fstp dword [FPU_R0]
        ret

    global FPU_log2
    FPU_log2:
        fld1
        fld  dword [FPU_S0]
        fyl2x
        fstp dword [FPU_R0]
        ret

    global FPU_tan
        FPU_tan:
		fld  dword [FPU_S0]
		fptan
	    fstp dword [FPU_R0]
	    fstp dword [FPU_R1]
        ret