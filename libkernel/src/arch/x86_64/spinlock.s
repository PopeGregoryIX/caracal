.globl __acquireLock
__acquireLock:

    lock btsq $0, (%rdi)
    jc .spinWait
    ret

.spinWait:
    pause
    testq $1, (%rdi)
    jnz .spinWait
    jmp __acquireLock

.globl __releaseLock
__releaseLock:
    movq $0, (%rdi)
    ret
