[BITS 64]
;	void __loadGdt(void* gdtr, uint8_t cs, uint8_t ds)
[GLOBAL __loadGdt]
__loadGdt:
	push	rbp
	mov		rbp,	rsp

	lgdt	[rdi]

	push	rsi
	mov		rax,	.__do64
	push	rax
	db	0x48					;	64 bit prefix
	retf
;
.__do64:
	mov		rax,	rdx
	mov		ds,		rax
	mov		es,		rax
	mov		fs,		rax
	mov		gs,		rax
	mov		ss,		rax

	pop		rbp
ret



