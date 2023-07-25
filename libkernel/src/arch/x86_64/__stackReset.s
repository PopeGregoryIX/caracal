.globl __stackReset
__stackReset:
    pop %rax
    mov %rdi, %rsp

    push %rax
    retq

