;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_jmp.s,v $
; $Date: 2003/06/19 23:58:25 $
; $Revision: 1.7 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__alloca_list|, WEAK
	IMPORT	|__executing_signalhandler|
	IMPORT	|__pthread_worksemaphore|
	IMPORT	free
	IMPORT	|_exit|
	IMPORT	|__trim_stack|

	; jmp_buf layout :
	;   +   0 ( 4) : contents __alloca_list
	;   +   4 ( 4) : __executing_signalhandler
	;   +   8 ( 4) : __pthread_worksemaphore
	;   +  12 (48) : f4, f5, f6, f7 on entry of setjmp()
	;   +  60 ( 4) : child process
	;   +  64 (24) : v1, v2, v3, v4, v5, v6 on entry of setjmp()
	;   +  88 ( 4) : sl
	;   +  92 ( 4) : fp
	;   +  96 ( 4) : sp
	;   + 100 ( 4) : lr
	; = 104 bytes (= __JMP_BUF_SIZE * sizeof(int))

	EXPORT	setjmp
	NAME	setjmp
setjmp
	; record the current allocation pointer for use with longjmp
	; Note, alloca_list is a weak symbol so may not be set
	[ __FEATURE_PTHREADS = 1
	IMPORT	|__pthread_running_thread|
	LDR	a4, =|__pthread_running_thread|
	LDR	a4, [a4]
	LDR	a4, [a4, #|__PTHREAD_ALLOCA_OFFSET| + 8]
	|
	LDR	a4, =|__alloca_list|
	TEQ	a4, #0
	LDRNE	a4, [a4, #0]
	]
	STR	a4, [a1], #4

	LDR	a4, =|__executing_signalhandler|
	LDR	a4, [a4]
	STR	a4, [a1], #4

	LDR	a4, =|__pthread_worksemaphore|
	LDR	a4, [a4]
	STR	a4, [a1], #4

	SFM	f4, 4, [a1], #4*12
	; WARNING :
	; Even though a1 does not need to be saved, this position in
	; the jmp_buf is used when a child process returns via vret
	STMIA	a1, {a1, v1, v2, v3, v4, v5, v6, sl, fp, sp, lr}

	MOV	a1, #0
	return	AL, pc, lr

	EXPORT	longjmp
	NAME	longjmp
longjmp
	; Free allocations that occurred after the setjmp. This must be
	; done before any registers, including the fp registers, are
	; restored from the jmp_buf, because free() could do anything.

	; We should be able to safely use v1-v6, since if a recursive
	; call to longjmp does occur, then the v1-v6 are going to be
	; safely restored to their current values.

	[ __FEATURE_PTHREADS = 1
	LDR	v5, =|__pthread_running_thread|
	LDR	v5, [v5]
	ADD	v5, v5, #|__PTHREAD_ALLOCA_OFFSET| + 8
	|
	LDR	v5, =|__alloca_list|
	]
	LDR	v4, [a1], #4
	CMP	v5, #0
	LDRNE	v3, [v5]
	MOV	v1, a1
	MOV	v2, a2
	CMPNE	v3, v4
	BEQ	|__longjmp_l3|

|__longjmp_l2|
	MOVS	a1, v3
	LDRNE	v3, [v3]		; StrongARM order
	BEQ	|__longjmp_fatal|	; oh fuck!, the list has run out
	; update pointers in loop.
	; must be done here in case free indirectly calls longjmp
	STR	v3, [v5]		; update head of alloca_list
	STR	v3, [v1, #-4]		; update the stored value in jmp_buf
	BL	free
	CMP	v3, v4
	BNE	|__longjmp_l2|

|__longjmp_l3|
	; Find the old stack pointer, and free any chunks
	LDR	sl, [v1, #88 - 4]
	LDR	sp, [v1, #96 - 4]
	BL	|__trim_stack|

	LDR	a1, =|__executing_signalhandler|
	LDR	lr, [v1], #4
	STR	lr, [a1]

	LDR	a1, =|__pthread_worksemaphore|
	LDR	lr, [v1], #4
	STR	lr, [a1]

	LFM	f4, 4, [v1], #4*12

	MOVS	a1, v2
	MOVEQ	a1, #1			; longjmp can't return 0
	; Technically there is a problem here if we should be
	; moving to a higher processor mode, such as USR -> SVC
	; WARNING :
	; While a1 probably doesn't need to be saved here, this
	; is what the old code did and UnixLib has already caught me out
	; once by using this position in the jmp_buf as the return
	; value for vret!!!!
	STR	a1, [v1]
	LDMIA	v1, {a1, v1, v2, v3, v4, v5, v6, sl, fp, sp, pc}

	; If we get here something has screwed up. The old value
	; of alloca list which we were searching for which was active
	; when the setjmp was called wasn't found. This can happen for
	; at least two reasons.
	;  1. The jmpbuf has become corrupt through being overwritten
	;  2. The user is attempting a longjmp to an invalid setjmp
	;     point, i.e., the scope of the setjmp has exited.
|__longjmp_fatal|
	ADR	a1, |__longjmp_fatal_msg|
	SWI	XOS_Write0
	; This failure is too serious to trying raising a signal, since
	; all the dynamic allocations have been freed and letting the user
	; access that memory is just too dangerous. Further, the abort code
	; or signal handler on abort may longjmp, causing recursion
	; until the stack runs out. This is all likely to crash the users
	; poor machine, so bale out now to be nice.
	MOV	a1, #EXIT_FAILURE
	B	|_exit|
|__longjmp_fatal_msg|
	DCB	13, 10
	DCB	13, 10
	DCB	"Fatal error detected in longjmp, jmpbuf possibly corrupt.",13,10
	DCB	"This can happen for at least two reasons.",13,10
	DCB	" - The jmpbuf has become corrupt through being overwritten.",13,10
	DCB	" - The program is attempting a longjmp to an invalid setjmp point",13,10
	DCB	"   i.e. the scope of the setjmp has exited."
	DCB	13, 10, 0

	END
