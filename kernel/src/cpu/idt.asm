[bits 64]

extern isr_dispatch


section .data
    align 16
    qtemp dq 0


section .text
    align 16

    %macro ctx_push 0
        ; ; cr3 -------------
        ; mov [qtemp], rax
        ;     mov rax, cr3
        ;     push rax
        ; mov rax, [qtemp]
        ; ; -----------------
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
        push r10
        push r9
        push r8
    %endmacro


    %macro ctx_pop 0
        pop r8
        pop r9
        pop r10
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
        ; ; cr3 -------------
        ; mov [qtemp], rax
        ;     pop rax
        ;     mov cr3, rax
        ; mov rax, [qtemp]
        ; ; -----------------
    %endmacro


    %macro _err_stub 1
    isr_stub_%+%1:
        cli
        push qword %1
        isr_stub_common
    %endmacro

    %macro _noerr_stub 1
    isr_stub_%+%1:
        cli
        push 0 ; Dummy error code
        push qword %1
        isr_stub_common
    %endmacro

    %macro isr_stub_common 0
        ctx_push
        mov rdi, rsp
        call isr_dispatch
        ctx_pop
        add rsp, 16
        sti
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
    _noerr_stub 128
    _noerr_stub 129
    _noerr_stub 130
    _noerr_stub 131
    _noerr_stub 132
    _noerr_stub 133
    _noerr_stub 134
    _noerr_stub 135
    _noerr_stub 136
    _noerr_stub 137
    _noerr_stub 138
    _noerr_stub 139
    _noerr_stub 140
    _noerr_stub 141
    _noerr_stub 142
    _noerr_stub 143
    _noerr_stub 144
    _noerr_stub 145
    _noerr_stub 146
    _noerr_stub 147
    _noerr_stub 148
    _noerr_stub 149
    _noerr_stub 150
    _noerr_stub 151
    _noerr_stub 152
    _noerr_stub 153
    _noerr_stub 154
    _noerr_stub 155
    _noerr_stub 156
    _noerr_stub 157
    _noerr_stub 158
    _noerr_stub 159
    _noerr_stub 160
    _noerr_stub 161
    _noerr_stub 162
    _noerr_stub 163
    _noerr_stub 164
    _noerr_stub 165
    _noerr_stub 166
    _noerr_stub 167
    _noerr_stub 168
    _noerr_stub 169
    _noerr_stub 170
    _noerr_stub 171
    _noerr_stub 172
    _noerr_stub 173
    _noerr_stub 174
    _noerr_stub 175
    _noerr_stub 176
    _noerr_stub 177
    _noerr_stub 178
    _noerr_stub 179
    _noerr_stub 180
    _noerr_stub 181
    _noerr_stub 182
    _noerr_stub 183
    _noerr_stub 184
    _noerr_stub 185
    _noerr_stub 186
    _noerr_stub 187
    _noerr_stub 188
    _noerr_stub 189
    _noerr_stub 190
    _noerr_stub 191
    _noerr_stub 192
    _noerr_stub 193
    _noerr_stub 194
    _noerr_stub 195
    _noerr_stub 196
    _noerr_stub 197
    _noerr_stub 198
    _noerr_stub 199
    _noerr_stub 200
    _noerr_stub 201
    _noerr_stub 202
    _noerr_stub 203
    _noerr_stub 204
    _noerr_stub 205
    _noerr_stub 206
    _noerr_stub 207
    _noerr_stub 208
    _noerr_stub 209
    _noerr_stub 210
    _noerr_stub 211
    _noerr_stub 212
    _noerr_stub 213
    _noerr_stub 214
    _noerr_stub 215
    _noerr_stub 216
    _noerr_stub 217
    _noerr_stub 218
    _noerr_stub 219
    _noerr_stub 220
    _noerr_stub 221
    _noerr_stub 222
    _noerr_stub 223
    _noerr_stub 224
    _noerr_stub 225
    _noerr_stub 226
    _noerr_stub 227
    _noerr_stub 228
    _noerr_stub 229
    _noerr_stub 230
    _noerr_stub 231
    _noerr_stub 232
    _noerr_stub 233
    _noerr_stub 234
    _noerr_stub 235
    _noerr_stub 236
    _noerr_stub 237
    _noerr_stub 238
    _noerr_stub 239
    _noerr_stub 240
    _noerr_stub 241
    _noerr_stub 242
    _noerr_stub 243
    _noerr_stub 244
    _noerr_stub 245
    _noerr_stub 246
    _noerr_stub 247
    _noerr_stub 248
    _noerr_stub 249
    _noerr_stub 250
    _noerr_stub 251
    _noerr_stub 252
    _noerr_stub 253
    _noerr_stub 254
    _noerr_stub 255















    global isrtab
    isrtab:
        dq isr_stub_0
        dq isr_stub_1
        dq isr_stub_2
        dq isr_stub_3
        dq isr_stub_4
        dq isr_stub_5
        dq isr_stub_6
        dq isr_stub_7
        dq isr_stub_8
        dq isr_stub_9
        dq isr_stub_10
        dq isr_stub_11
        dq isr_stub_12
        dq isr_stub_13
        dq isr_stub_14
        dq isr_stub_15
        dq isr_stub_16
        dq isr_stub_17
        dq isr_stub_18
        dq isr_stub_19
        dq isr_stub_20
        dq isr_stub_21
        dq isr_stub_22
        dq isr_stub_23
        dq isr_stub_24
        dq isr_stub_25
        dq isr_stub_26
        dq isr_stub_27
        dq isr_stub_28
        dq isr_stub_29
        dq isr_stub_30
        dq isr_stub_31
        dq isr_stub_32
        dq isr_stub_33
        dq isr_stub_34
        dq isr_stub_35
        dq isr_stub_36
        dq isr_stub_37
        dq isr_stub_38
        dq isr_stub_39
        dq isr_stub_40
        dq isr_stub_41
        dq isr_stub_42
        dq isr_stub_43
        dq isr_stub_44
        dq isr_stub_45
        dq isr_stub_46
        dq isr_stub_47
        dq isr_stub_48
        dq isr_stub_49
        dq isr_stub_50
        dq isr_stub_51
        dq isr_stub_52
        dq isr_stub_53
        dq isr_stub_54
        dq isr_stub_55
        dq isr_stub_56
        dq isr_stub_57
        dq isr_stub_58
        dq isr_stub_59
        dq isr_stub_60
        dq isr_stub_61
        dq isr_stub_62
        dq isr_stub_63
        dq isr_stub_64
        dq isr_stub_65
        dq isr_stub_66
        dq isr_stub_67
        dq isr_stub_68
        dq isr_stub_69
        dq isr_stub_70
        dq isr_stub_71
        dq isr_stub_72
        dq isr_stub_73
        dq isr_stub_74
        dq isr_stub_75
        dq isr_stub_76
        dq isr_stub_77
        dq isr_stub_78
        dq isr_stub_79
        dq isr_stub_80
        dq isr_stub_81
        dq isr_stub_82
        dq isr_stub_83
        dq isr_stub_84
        dq isr_stub_85
        dq isr_stub_86
        dq isr_stub_87
        dq isr_stub_88
        dq isr_stub_89
        dq isr_stub_90
        dq isr_stub_91
        dq isr_stub_92
        dq isr_stub_93
        dq isr_stub_94
        dq isr_stub_95
        dq isr_stub_96
        dq isr_stub_97
        dq isr_stub_98
        dq isr_stub_99
        dq isr_stub_100
        dq isr_stub_101
        dq isr_stub_102
        dq isr_stub_103
        dq isr_stub_104
        dq isr_stub_105
        dq isr_stub_106
        dq isr_stub_107
        dq isr_stub_108
        dq isr_stub_109
        dq isr_stub_110
        dq isr_stub_111
        dq isr_stub_112
        dq isr_stub_113
        dq isr_stub_114
        dq isr_stub_115
        dq isr_stub_116
        dq isr_stub_117
        dq isr_stub_118
        dq isr_stub_119
        dq isr_stub_120
        dq isr_stub_121
        dq isr_stub_122
        dq isr_stub_123
        dq isr_stub_124
        dq isr_stub_125
        dq isr_stub_126
        dq isr_stub_127
        dq isr_stub_128
        dq isr_stub_129
        dq isr_stub_130
        dq isr_stub_131
        dq isr_stub_132
        dq isr_stub_133
        dq isr_stub_134
        dq isr_stub_135
        dq isr_stub_136
        dq isr_stub_137
        dq isr_stub_138
        dq isr_stub_139
        dq isr_stub_140
        dq isr_stub_141
        dq isr_stub_142
        dq isr_stub_143
        dq isr_stub_144
        dq isr_stub_145
        dq isr_stub_146
        dq isr_stub_147
        dq isr_stub_148
        dq isr_stub_149
        dq isr_stub_150
        dq isr_stub_151
        dq isr_stub_152
        dq isr_stub_153
        dq isr_stub_154
        dq isr_stub_155
        dq isr_stub_156
        dq isr_stub_157
        dq isr_stub_158
        dq isr_stub_159
        dq isr_stub_160
        dq isr_stub_161
        dq isr_stub_162
        dq isr_stub_163
        dq isr_stub_164
        dq isr_stub_165
        dq isr_stub_166
        dq isr_stub_167
        dq isr_stub_168
        dq isr_stub_169
        dq isr_stub_170
        dq isr_stub_171
        dq isr_stub_172
        dq isr_stub_173
        dq isr_stub_174
        dq isr_stub_175
        dq isr_stub_176
        dq isr_stub_177
        dq isr_stub_178
        dq isr_stub_179
        dq isr_stub_180
        dq isr_stub_181
        dq isr_stub_182
        dq isr_stub_183
        dq isr_stub_184
        dq isr_stub_185
        dq isr_stub_186
        dq isr_stub_187
        dq isr_stub_188
        dq isr_stub_189
        dq isr_stub_190
        dq isr_stub_191
        dq isr_stub_192
        dq isr_stub_193
        dq isr_stub_194
        dq isr_stub_195
        dq isr_stub_196
        dq isr_stub_197
        dq isr_stub_198
        dq isr_stub_199
        dq isr_stub_200
        dq isr_stub_201
        dq isr_stub_202
        dq isr_stub_203
        dq isr_stub_204
        dq isr_stub_205
        dq isr_stub_206
        dq isr_stub_207
        dq isr_stub_208
        dq isr_stub_209
        dq isr_stub_210
        dq isr_stub_211
        dq isr_stub_212
        dq isr_stub_213
        dq isr_stub_214
        dq isr_stub_215
        dq isr_stub_216
        dq isr_stub_217
        dq isr_stub_218
        dq isr_stub_219
        dq isr_stub_220
        dq isr_stub_221
        dq isr_stub_222
        dq isr_stub_223
        dq isr_stub_224
        dq isr_stub_225
        dq isr_stub_226
        dq isr_stub_227
        dq isr_stub_228
        dq isr_stub_229
        dq isr_stub_230
        dq isr_stub_231
        dq isr_stub_232
        dq isr_stub_233
        dq isr_stub_234
        dq isr_stub_235
        dq isr_stub_236
        dq isr_stub_237
        dq isr_stub_238
        dq isr_stub_239
        dq isr_stub_240
        dq isr_stub_241
        dq isr_stub_242
        dq isr_stub_243
        dq isr_stub_244
        dq isr_stub_245
        dq isr_stub_246
        dq isr_stub_247
        dq isr_stub_248
        dq isr_stub_249
        dq isr_stub_250
        dq isr_stub_251
        dq isr_stub_252
        dq isr_stub_253
        dq isr_stub_254
        dq isr_stub_255



