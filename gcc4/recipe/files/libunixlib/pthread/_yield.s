@ Yield processor control to another thread
@ Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
@ Written by Martin Piper and Alex Waugh

#include "unixlib/asm_dec.s"

	.text


@ pthread_yield
@ Called in USR mode by a thread wishing to give up the processor
	.global	pthread_yield
	NAME	pthread_yield
pthread_yield:
	@ Setup an APCS-32 stack frame so this will appear in a backtrace
	MOV	ip, sp
 PICNE "STMFD	sp!, {fp, ip, lr, pc}"
 PICEQ "STMFD	sp!, {v4, fp, ip, lr, pc}"

	SUB	fp, ip, #4

 PICEQ "LDR	v4, .L0+8"
.LPIC0:
 PICEQ "ADD	v4, pc, v4"		@ v4 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	v4, {v4, v5}"		@ v4 = Object index, v5 = GOT array location
 PICEQ "LDR	v5, [v5, #0]"		@ v5 = GOT array
 PICEQ "LDR	v4, [v5, v4, LSL#2]"	@ v4 = GOT (private)

	@ If the thread system isn't running then yielding is pointless
	LDR	a2, .L0	@=__ul_global
 PICEQ "LDR	a2, [v4, a2]"
	LDR	a1, [a2, #GBL_PTH_SYSTEM_RUNNING]
	CMP	a1, #0
 PICEQ "LDMEQDB fp, {v4, fp, sp, pc}"
 PICNE "LDMEQDB	fp, {fp, sp, pc}"

	@ Check that a context switch can take place
	LDR	a1, [a2, #GBL_PTH_WORKSEMAPHORE]
	CMP	a1, #0
	ADRNE	a1, failmessage
	BLNE	__pthread_fatal_error

	MRS	a3, CPSR	@ Get a USR mode, IRQs enabled CPSR
	@ Change to SVC mode, IRQs disabled to ensure the regs don't get
	@ overwitten by a real callback
	SWI	XOS_EnterOS
	CHGMODE	a4, SVC_Mode+IFlag
	@ Save regs to callback save area
	LDR	a1, .L0+4	@=__cbreg
 PICEQ "LDR	a1, [v4, a1]"
	ADD	a1, a1, #15*4
	STMDB	a1, {r4-r14}^
	ADR	a2, __pthread_yield_return	@USR mode, IRQs enabled if in a 26bit mode
	STR	a2, [a1]
	STR	a3, [a1, #4]	@ Save CPSR

	@ Branch directly to the context switcher.
	@ It will return by loading the registers from __cbreg
	B	__pthread_callback

	NAME	__pthread_yield_return
__pthread_yield_return:
 PICEQ "LDMDB	fp, {v4, fp, sp, pc}"
 PICNE "LDMDB	fp, {fp, sp, pc}"
.L0:
	WORD	__ul_global
	WORD	__cbreg
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"

failmessage:
	.asciz	"pthread_yield called with context switching disabled"
	.align
	DECLARE_FUNCTION pthread_yield

	.end
