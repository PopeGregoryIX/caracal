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
    mov %cr3, %rax              //   keep BSP and AP memory maps consistent (otherwise _bsp_init_done is out of sync)
    mov %rax, %cr3

    mov $_bsp_init_done, %rax
    mov (%rax), %rcx
    jmp .ap


.globl _setBspDone
_setBspDone:
    push %rax
    push %rcx

    mov $_bsp_init_done, %rax
    mov $0xFFFFFFFFFFFFFFFF, %rcx
    mov %rcx, (%rax)

    pop %rcx
    pop %rax
    ret

.align 4
.data
_bsp_init_done:
.8byte 0x0000000000000000
