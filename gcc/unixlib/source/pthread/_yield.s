; Yield processor control to another thread
; Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
; Written by Martin Piper and Alex Waugh

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__pthread_fatal_error|
	IMPORT	|__pthread_callback|
	IMPORT	|__cbreg|
	IMPORT	|__ul_global|

; pthread_yield
; Called in USR mode by a thread wishing to give up the processor
	EXPORT	|pthread_yield|
	NAME	pthread_yield
|pthread_yield|
	; Setup an APCS-32 stack frame so this will appear in a backtrace
	MOV	ip, sp
 PICEQ "STMFD	sp!, {v4, fp, ip, lr, pc}"
 PICEQ "BL	__rt_load_pic"
 PICNE "STMFD	sp!, {fp, ip, lr, pc}"
	SUB	fp, ip, #4

	; If the thread system isn't running then yielding is pointless
	LDR	a2, |.L0|	;=__ul_global
 PICEQ "LDR	a2, [v4, a2]"
	LDR	a1, [a2, #GBL_PTH_SYSTEM_RUNNING]
	CMP	a1, #0
 PICEQ "LDMEQDB fp, {v4, fp, sp, pc}"
 PICNE "LDMEQDB	fp, {fp, sp, pc}"

	; Check that a context switch can take place
	LDR	a1, [a2, #GBL_PTH_WORKSEMAPHORE]
	CMP	a1, #0
	ADRNE	a1, failmessage
	BLNE	|__pthread_fatal_error|

	MRS	a3, CPSR	; Get a USR mode, IRQs enabled CPSR
	; Change to SVC mode, IRQs disabled to ensure the regs don't get
	; overwitten by a real callback
	SWI	XOS_EnterOS
	CHGMODE	a4, SVC_Mode+IFlag
	; Save regs to callback save area
	LDR	a1, |.L0|+4	;=__cbreg
 PICEQ "LDR	a1, [v4, a1]"
	ADD	a1, a1, #15*4
	STMDB	a1, {r4-r14}^
	ADR	a2, __pthread_yield_return	;USR mode, IRQs enabled if in a 26bit mode
	STR	a2, [a1]
	STR	a3, [a1, #4]	; Save CPSR

	; Branch directly to the context switcher.
	; It will return by loading the registers from __cbreg
	B	__pthread_callback

	NAME	__pthread_yield_return
__pthread_yield_return
 PICEQ "LDMDB	fp, {v4, fp, sp, pc}"
 PICNE "LDMDB	fp, {fp, sp, pc}"
|.L0|
	WORD	|__ul_global|
	WORD	|__cbreg|

failmessage
	DCB	"pthread_yield called with context switching disabled", 0
	ALIGN
	DECLARE_FUNCTION pthread_yield

	END
