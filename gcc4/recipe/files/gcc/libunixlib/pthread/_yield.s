@ Yield processor control to another thread
@ Written by Martin Piper and Alex Waugh
@ Copyright (c) 2002, 2003, 2004, 2005, 2006, 2008, 2009, 2019 UnixLib Developers

#include "internal/asm_dec.s"

	.syntax unified
	.text

@ pthread_yield
@ Called in USR mode by a thread wishing to give up the processor
	.global	pthread_yield
	NAME	pthread_yield
pthread_yield:
#ifdef __ARM_EABI__
	STMFD	sp!, {fp, lr}
#ifdef __clang__
	MOV	fp, sp
#else
	ADD	fp, sp, #4
#endif
#else
	@ Setup an APCS-32 stack frame so this will appear in a backtrace
	MOV	ip, sp
	STMFD	sp!, {fp, ip, lr, pc}

	SUB	fp, ip, #4
#endif
	PIC_LOAD a4

	@ If the thread system isn't running then yielding is pointless
	LDR	a3, .L0		@=__ul_global
 PICEQ "LDR	a3, [a4, a3]"
	LDR	a1, [a3, #GBL_PTH_SYSTEM_RUNNING]
	CMP	a1, #0
#ifdef __ARM_EABI__
	LDMIAEQ	sp!, {fp, pc}
#else
	LDMDBEQ	fp, {fp, sp, pc}
#endif

	@ Check that a context switch can take place
	LDR	a2, [a3, #GBL_PTH_CALLEVERY_RMA]
	LDR	a1, [a2, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE]
	CMP	a1, #0
	ADRNE	a1, failmessage
	BLNE	__pthread_fatal_error
0:
	MOV	a1, a2	@ callback regs are located at the start of the fast access RMA block

	MRS	a3, CPSR	@ Get a USR mode, IRQs enabled CPSR
	@ Change to SVC mode, IRQs disabled to ensure the regs don't get
	@ overwitten by a real callback
	SWI	XOS_EnterOS
	CHGMODE	a4, SVC_Mode+IFlag

	@ Save regs to callback save area
	ADD	a1, a1, #13*4

	@ Using an STMDB instruction to save all relevant registers can trigger
	@ a bug in revision K StrongARM CPUs whereby, as in this particular case,
	@ banked register r14 is not saved. To avoid it, split the instruction
	@ into two so that the banked registers are saved independently of the
	@ others.
	STMDB	a1, {r4-r12}
	STMIA	a1, {r13,r14}^
	ADR	a2, __pthread_yield_return	@USR mode, IRQs enabled if in a 26bit mode
	STR	a2, [a1, #8]
	STR	a3, [a1, #12]	@ Save CPSR

	@ Branch directly to the context switcher.
	@ It will return by loading the registers from the fast access RMA block
	B	__pthread_callback

	NAME	__pthread_yield_return
__pthread_yield_return:
#ifdef __ARM_EABI__
	LDMIA	sp!, {fp, pc}
#else
	LDMDB	fp, {fp, sp, pc}
#endif
.L0:
	WORD	__ul_global

failmessage:
	.asciz	"pthread_yield called with context switching disabled"
	.align
	DECLARE_FUNCTION pthread_yield

	.end
