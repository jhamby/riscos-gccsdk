;----------------------------------------------------------------------------
;
; $Source$
; $Date$
; $Revision$
; $State$
; $Author$
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__vfork|
	IMPORT	|__vexit|
	IMPORT	|__dynamic_num|
	IMPORT	|__real_break|

	IMPORT	setjmp
	IMPORT	longjmp

	EXPORT	vfork
	NAME	vfork
vfork
	STMFD	sp!, {v1, lr}
	BL	|__vfork|
	; If zero was returned, we will return -1
	CMP	a1, #0
	MVNEQ	a1, #0
	stackreturn	EQ, "v1, pc"
	LDMFD	sp!, {v1, lr}
	; save lr for use when setjmp returns either immediately or
	; via longjmp as spawned program exits and current program
	; starts running again.
	LDR	a2, =|__saved_lr|
	STR	lr, [a2]
	BL	setjmp
	LDR	a2, =|__saved_lr|
	TEQ	a1, #0
	LDR	lr, [a2]
	return	EQ, pc, lr

	; Restore dynamic area size to size indicated by __real_break
	; which was the size of the area before the forked program
	; ran and extended the dynamic area. We can't do this anywhere
	; before this point (e.g., in the called program) since some
	; pointers may need copying down from the spawned program's area.
	STMFD	sp!,{a1,a2,a3,a4,v1,v2,v3,v4,v5,lr}
	LDR	a2, =|__dynamic_num|
	LDR	a2, [a2]
	CMN	a2, #1
	stackreturn	EQ, "a1,a2,a3,a4,v1,v2,v3,v4,v5,pc"
	MOV	a1, #2
	SWI	XOS_DynamicArea
	MOVVC	a1, a2
	LDRVC	a2, =|__real_break|
	ADDVC	a3, a3, a4
	LDRVC	a2, [a2]
	SUBVC	a2, a2, a3
	SWIVC	XOS_ChangeDynamicArea
	stackreturn	AL, "a1,a2,a3,a4,v1,v2,v3,v4,v5,pc"

	EXPORT	|__vret|
|__vret|
	BL	|__vexit|
	LDR	a2, [a1, #52]
	B	longjmp

	AREA	|C$$zidata|, DATA, NOINIT
|__saved_lr|
	%	4

	END
