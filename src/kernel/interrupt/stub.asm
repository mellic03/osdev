[bits 64]


; global __cpu_saved_state
; __cpu_saved_state:
;     __cpu_rax dq 0
;     __cpu_rbx dq 0
;     __cpu_rcx dq 0
;     __cpu_rdx dq 0
;     __cpu_rdi dq 0
;     __cpu_rsi dq 0
;     __cpu_rbp dq 0
;     __cpu_cr0 dq 0
;     __cpu_cr1 dq 0
;     __cpu_cr2 dq 0
;     __cpu_cr3 dq 0
;     __cpu_vcode dq 0
;     __cpu_ecode dq 0


; %macro cpu_pushall 0
;     mov [__cpu_rax], rax
;     mov [__cpu_rbx], rbx
;     mov [__cpu_rcx], rcx
;     mov [__cpu_rdx], rdx
;     mov [__cpu_rdi], rdi
;     mov [__cpu_rsi], rsi
;     mov [__cpu_rbp], rbp
;     mov rax, cr0
;     mov [__cpu_cr0], rax
;     mov rax, cr1
;     mov [__cpu_cr1], rax
;     mov rax, cr2
;     mov [__cpu_cr2], rax
;     mov rax, cr3
;     mov [__cpu_cr3], rax

;     mov rax, $1
;     mov [__cpu_vcode], rax
;     mov rax, 1234
;     mov [__cpu_ecode], rax

;     mov rax, [__cpu_rax]

; %endmacro




section .text
extern __isr_dispatch



get_rip:
    mov rax, [rsp]
    ret


%macro ctx_push 0
    push rbp
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax
    push r15
    push r14
    push r13
    push r12
    push r11
%endmacro


%macro ctx_pop 0
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
    pop rbp
%endmacro


%macro _err_stub 1
isr_stub_%+%1:
    push qword %1
    isr_stub_common
%endmacro

%macro _noerr_stub 1
isr_stub_%+%1:
    push 0 ; Dummy error code
    push qword %1
    isr_stub_common
%endmacro

%macro isr_stub_common 0
    ctx_push
    mov rdi, rsp
    call __isr_dispatch
    ctx_pop
    add rsp, 16
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
; _noerr_stub 100
; _noerr_stub 101
; _noerr_stub 102
; _noerr_stub 103
; _noerr_stub 104
; _noerr_stub 105
; _noerr_stub 106
; _noerr_stub 107
; _noerr_stub 108
; _noerr_stub 109
; _noerr_stub 110
; _noerr_stub 111
; _noerr_stub 112
; _noerr_stub 113
; _noerr_stub 114
; _noerr_stub 115
; _noerr_stub 116
; _noerr_stub 117
; _noerr_stub 118
; _noerr_stub 119
; _noerr_stub 120
; _noerr_stub 121
; _noerr_stub 122
; _noerr_stub 123
; _noerr_stub 124
; _noerr_stub 125
; _noerr_stub 126
; _noerr_stub 127
; _noerr_stub 128
; _noerr_stub 129
; _noerr_stub 130
; _noerr_stub 131
; _noerr_stub 132
; _noerr_stub 133
; _noerr_stub 134
; _noerr_stub 135
; _noerr_stub 136
; _noerr_stub 137
; _noerr_stub 138
; _noerr_stub 139
; _noerr_stub 140
; _noerr_stub 141
; _noerr_stub 142
; _noerr_stub 143
; _noerr_stub 144
; _noerr_stub 145
; _noerr_stub 146
; _noerr_stub 147
; _noerr_stub 148
; _noerr_stub 149
; _noerr_stub 150
; _noerr_stub 151
; _noerr_stub 152
; _noerr_stub 153
; _noerr_stub 154
; _noerr_stub 155
; _noerr_stub 156
; _noerr_stub 157
; _noerr_stub 158
; _noerr_stub 159
; _noerr_stub 160
; _noerr_stub 161
; _noerr_stub 162
; _noerr_stub 163
; _noerr_stub 164
; _noerr_stub 165
; _noerr_stub 166
; _noerr_stub 167
; _noerr_stub 168
; _noerr_stub 169
; _noerr_stub 170
; _noerr_stub 171
; _noerr_stub 172
; _noerr_stub 173
; _noerr_stub 174
; _noerr_stub 175
; _noerr_stub 176
; _noerr_stub 177
; _noerr_stub 178
; _noerr_stub 179
; _noerr_stub 180
; _noerr_stub 181
; _noerr_stub 182
; _noerr_stub 183
; _noerr_stub 184
; _noerr_stub 185
; _noerr_stub 186
; _noerr_stub 187
; _noerr_stub 188
; _noerr_stub 189
; _noerr_stub 190
; _noerr_stub 191
; _noerr_stub 192
; _noerr_stub 193
; _noerr_stub 194
; _noerr_stub 195
; _noerr_stub 196
; _noerr_stub 197
; _noerr_stub 198
; _noerr_stub 199
; _noerr_stub 200
; _noerr_stub 201
; _noerr_stub 202
; _noerr_stub 203
; _noerr_stub 204
; _noerr_stub 205
; _noerr_stub 206
; _noerr_stub 207
; _noerr_stub 208
; _noerr_stub 209
; _noerr_stub 210
; _noerr_stub 211
; _noerr_stub 212
; _noerr_stub 213
; _noerr_stub 214
; _noerr_stub 215
; _noerr_stub 216
; _noerr_stub 217
; _noerr_stub 218
; _noerr_stub 219
; _noerr_stub 220
; _noerr_stub 221
; _noerr_stub 222
; _noerr_stub 223
; _noerr_stub 224
; _noerr_stub 225
; _noerr_stub 226
; _noerr_stub 227
; _noerr_stub 228
; _noerr_stub 229
; _noerr_stub 230
; _noerr_stub 231
; _noerr_stub 232
; _noerr_stub 233
; _noerr_stub 234
; _noerr_stub 235
; _noerr_stub 236
; _noerr_stub 237
; _noerr_stub 238
; _noerr_stub 239
; _noerr_stub 240
; _noerr_stub 241
; _noerr_stub 242
; _noerr_stub 243
; _noerr_stub 244
; _noerr_stub 245
; _noerr_stub 246
; _noerr_stub 247
; _noerr_stub 248
; _noerr_stub 249
; _noerr_stub 250
; _noerr_stub 251
; _noerr_stub 252
; _noerr_stub 253
; _noerr_stub 254
; _noerr_stub 255



global __isr_table
__isr_table:
%assign i 0 
%rep    100
    dq isr_stub_%+i
%assign i i+1 
%endrep
