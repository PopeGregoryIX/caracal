.globl _entry
.extern kmain
_entry:
    mov $1, %rax
    cpuid
    shr $24, %rbx
    cmpw %bx, bootboot + 0xC
    jne .ap

    jmp kmain

.ap:
    mov $_bsp_init_done, %rax
    mov (%rax), %rcx
    jmp .ap


.align 4
.data
.globl _bsp_init_done
_bsp_init_done:
.8byte 0x0000000000000000
