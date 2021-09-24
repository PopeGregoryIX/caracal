//	void __loadIdt(void* idtr)
.globl __loadIdt
__loadIdt:
	push	%rbp
	movq	%rsp,	%rbp

	lidt	(%rdi)

	pop		%rbp
ret



