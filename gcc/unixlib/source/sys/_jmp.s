;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/sys/s/_jmp,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.11 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__fpflag|
	IMPORT	|__alloca_list|,WEAK
	IMPORT	free
	IMPORT	|_exit|

	EXPORT	setjmp
setjmp
	; record the current allocation pointer for use with longjmp
	; Note, alloca_list is a weak symbol so may not be set
	; Warning!!!
	; Do not change alloca_list so that it is not weak
	; because it is weak in _syslib.s and it would confuse the linker
	LDR	a4, =|__alloca_list|
	CMP	a4, #0
	LDRNE	a4, [a4]
	STR	a4, [a1], #4

	LDR	a4, =|__fpflag|
	LDR	a4, [a4]
	CMP	a4, #0
	ADDEQ	a1, a1, #48
	BEQ	|__setjmp_l1|
	[	ARMFPE = "3"
	SFM	f4, 4, [a1], #12
	|
	STFE	f4, [a1], #12
	STFE	f5, [a1], #12
	STFE	f6, [a1], #12
	STFE	f7, [a1], #12
	]
|__setjmp_l1|
	; warning!!!!
	; even though a1 does not need to be saved, this position in
	; the jmp_buf is used when a child process returns via vret
	STMIA	a1,{a1,v1,v2,v3,v4,v5,v6,fp,sp,lr}

	MOV	a1, #0
	return	AL, pc, lr

	EXPORT	longjmp
longjmp
	; free allocations that occurred after the setjmp. This must be
	; done before any registers, including the fp registers, are
	; restored from the jmp_buf, because free() could do anything.

	; we should be able to safely use v1-v6, since if a recursive
	; call to longjmp does occur, then the v1-v6 are going to be
	; safely restored to their current values.

	; what about sl?, should __stack be reset ?

	LDR	v5, =|__alloca_list|
	LDR	v4, [a1], #4
	CMP	v5, #0
	LDRNE	v3, [v5]
	MOV	v1, a1
	CMPNE	v3, v4
	BEQ	|__longjmp_l3|
	MOV	v2, a2
|__longjmp_l2|
	MOVS	a1, v3
	LDRNE	v3, [v3]			; StrongARM order
	BEQ	|__longjmp_fatal|	; oh fuck!, the list has run out
	; update pointers in loop.
	; must be done here in case free indirectly calls longjmp
	STR	v3, [v5]		; update head of alloca_list
	STR	v3, [v1,#-4]		; update the stored value in jmp_buf
	BL	free
	CMP	v3, v4
	BNE	|__longjmp_l2|
	MOV	a2, v2
|__longjmp_l3|
	LDR	a4, =|__fpflag|
	LDR	a4, [a4]
	CMP	a4, #0
	ADDEQ	v1, v1, #48
	BEQ	|__longjmp_l1|
	[	ARMFPE = "3"
	LFM	f4, 4, [v1], #12
	|	
	LDFE	f4, [v1], #12
	LDFE	f5, [v1], #12
	LDFE	f6, [v1], #12
	LDFE	f7, [v1], #12
	]
|__longjmp_l1|
	MOVS	a1, a2
	MOVEQ	a1, #1			; longjmp can't return 0
	; technically there is a problem here if we should be
	; moving to a higher processor mode, such as USR -> SVC
	; warning!!!!
	; while a1 probably doesn't need to be saved here, this
	; is what the old code did and UnixLib has already caught me out
	; once by using this position in the jmp_buf as the return
	; value for vret!!!!
	STR	a1, [v1]
	[	APCS32 = "no"
	LDMIA	v1, {a1, v1, v2, v3, v4, v5, v6, fp, sp, pc}^
	|
	LDMIA	v1, {a2, v1, v2, v3, v4, v5, v6, fp, sp, pc}
	]

	; if we get here something has screwed up. The old value
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
