.global switchTask
switchTask:
    # Save general-purpose registers
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    # Save flags
    pushfq

    # Save CR3
    mov %cr3, %rax
    push %rax

    # Save current task state
    mov 72(%rsp), %rax        # First argument: pointer to old task structure
    mov %rax, (%rax)          # Save RAX
    mov %rbx, 8(%rax)         # Save RBX
    mov %rcx, 16(%rax)        # Save RCX
    mov %rdx, 24(%rax)        # Save RDX
    mov %rsi, 32(%rax)        # Save RSI
    mov %rdi, 40(%rax)        # Save RDI
    mov %rbp, 48(%rax)        # Save RBP
    mov %rsp, 56(%rax)        # Save RSP
    mov (%rsp), %rbx          # Save RIP (top of stack)
    mov %rbx, 64(%rax)        # Save RIP to task structure
    pop %rbx                  # Restore stack
    pushfq                    # Save FLAGS onto the stack
    mov (%rsp), %rbx          # Move FLAGS from stack to RBX
    mov %rbx, 72(%rax)        # Save FLAGS to task structure
    pop %rbx                  # Restore stack
    mov (%rsp), %rbx          # Save CR3
    mov %rbx, 80(%rax)        # Save CR3 to task structure
    pop %rbx                  # Restore stack

    # Restore new task state
    mov 80(%rsp), %rax        # Second argument: pointer to new task structure
    mov (%rax), %rbx          # Load RBX
    mov 8(%rax), %rbx         # Load RCX
    mov 16(%rax), %rdx        # Load RDX
    mov 24(%rax), %rsi        # Load RSI
    mov 32(%rax), %rdi        # Load RDI
    mov 48(%rax), %rbp        # Load RBP
    mov 56(%rax), %rsp        # Load RSP
    push 64(%rax)             # Push new RIP onto the stack
    pushq 72(%rax)            # Push new FLAGS onto the stack
    popfq                     # Restore FLAGS
    pop %rbx                  # Restore stack
    mov 80(%rax), %rbx        # Load new CR3 into RBX
    mov %rbx, %cr3            # Restore CR3

    # Restore general-purpose registers
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax

    jmp *(%rsp)               # Jump to the new RIP
