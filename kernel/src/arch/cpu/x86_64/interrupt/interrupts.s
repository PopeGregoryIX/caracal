/**
 * \file exceptions.s
 *
 * \date Feb 10, 2012
 * \author AJSoft (Adam Jones)
 * \brief
 */

// The ISR macro is used for exceptions that do not push an error code
.macro ISR num
.globl isr\num
isr\num:
pushq	$0
pushq	$\num

push    %gs
push    %fs

pushaq

mov		%rsp,	%rdi
mov		__exception_call_table + (\num * 8),	%rax
call	*%rax
mov		%rdi,	%rsp

popaq

pop     %fs
pop     %gs

add		$0x16,	%esp
iretq
.endm

//	The ISR_EC macro is used where the exception automatically pushes an error code
.macro ISR_EC num
.globl isr\num
isr\num:
pushq	$\num

push    %gs
push    %fs

pushaq

mov		%rsp,	%rdi
mov		__exception_call_table + (\num * 8),	%rax
call	*%rax
mov		%rdi,	%rsp

popaq

pop     %fs
pop     %gs
add		$0x16,	%esp
iretq
.endm

// The IRQ macro is used for IRQ handlers
.macro IRQ num
.globl irq\num
irq\num:
pushq	$0
pushq	$\num

push    %gs
push    %fs

pushaq

mov		%rsp,	%rdi
mov		__irq_call_table + (\num * 8),	%rax
call	*%rax
mov		%rdi,	%rsp

popaq

pop     %fs
pop     %gs

add		$0x16,	%esp
iretq
.endm

.macro pushaq
    push %rax
    push %rcx
    push %rdx
    push %rbx
    push %rbp
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
.endm # pushaq

.macro popaq
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %r11
	pop %r10
	pop %r9
	pop %r8
    pop %rdi
    pop %rsi
    pop %rbp
    pop %rbx
    pop %rdx
    pop %rcx
    pop %rax
.endm # popaq

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_EC 8
ISR 9
ISR_EC 10
ISR_EC 11
ISR_EC 12
ISR_EC 13
ISR_EC 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31

.align 4
.data
.globl __exception_call_table
__exception_call_table:
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE     //  ISR0 - Exception 0
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch10Exceptions14DefaultHandlerEPNS_9RegistersE     // ISR 32 - Exception 32

.globl __irq_call_table
__irq_call_table:
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     //  ISR33 - IRQ 0
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     // ISR 64 - IRQ32

.globl __interrupt_call_table
__interrupt_call_table:
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     //  ISR65
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     // ISR 96

.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     //  ISR97
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     // ISR 128

.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     //  ISR 129
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     // ISR 160

.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     //  ISR161
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     // ISR 192

.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     //  ISR 193
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     // ISR 224

.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     //  ISR 225
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE
.8byte _ZN4arch3Idt14DefaultHandlerEPNS_9RegistersE     // ISR 256
.end
