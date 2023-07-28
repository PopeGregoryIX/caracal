.globl __stackReset
__stackReset:
    pop %rax
    mov %rdi, %rsp

    push %rax
    retq
    mov %rdi, %rsp  /*  move the stack */
    mov %rdx, %rdi  /*  pass a parameter to the called function */
    push %rsi
    ret             /*  call the new entry point    */
    