.globl __loadGdt
__loadGdt:
    push    %rax
    lgdt    (%rdi)

    mov     $0x10,  %rax
    mov     %ax,    %ds
    mov     %ax,    %es
    mov     %ax,    %fs
    mov     %ax,    %gs
    mov     %ax,    %ss

    mov     $__do64, %rax
    push    %rax
    .byte      0x48
    retq

__do64:
    pop     %rax
    ret
