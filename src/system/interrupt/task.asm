
%macro PushAll 0
	push rsp
	pushfq
	push rbp
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro PopAll 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop rbp
	popfq
	add rsp, 8; simulate pop rsp(see push rsp in PushAll)
%endmacro

SwitchContext:
	;rdi = nextTask->Rsp
	;rsi = &currTask->Rsp
	;rdx = complete callback
	;rcx = complete callback ctx
	PushAll
	mov rax, rsp
	mov rsp, rdi
	mov [rsi], rax
	mov rdi, rcx
	call rdx ; call complete callback
	PopAll
	sti
	ret





SetJmp:
	pop rsi
	xor eax, eax
	mov qword [rdi], rbx
	mov qword [rdi + 8h], rsp
	push rsi
	mov qword [rdi + 10h], rbp
	mov qword [rdi + 18h], r12
	mov qword [rdi + 20h], r13
	mov qword [rdi + 28h], r14
	mov qword [rdi + 30h], r15
	mov qword [rdi + 38h], rsi
	ret

LongJmp:
	mov rax, rsi
	mov rbx, qword [rdi]
	mov rsp, qword [rdi + 8h]
	mov rbp, qword [rdi + 10h]
	mov r12, qword [rdi + 18h]
	mov r13, qword [rdi + 20h]
	mov r14, qword [rdi + 28h]
	mov r15, qword [rdi + 30h]
	jmp qword [rdi + 38h]

