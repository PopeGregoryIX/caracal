.globl __stackReset
__stackReset:
    mov %rdi, %rsp

    mov %rdi, %rsp  /*  move the stack */
    mov %rdx, %rdi  /*  pass a parameter to the called function */
    push %rsi
    ret             /*  call the new entry point    */
    