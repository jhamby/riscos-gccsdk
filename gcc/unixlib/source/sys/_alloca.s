;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_alloca.s,v $
; $Date: 2001/09/11 13:05:55 $
; $Revision: 1.3.2.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT  malloc
	IMPORT  free
	IMPORT  |__base|
	IMPORT  abort

	EXPORT  |__alloca|
	EXPORT  alloca
alloca		; just in case
|__alloca|
	CMP	a1, #0
	return	EQ, pc, lr

	ADD	a1, a1, #8
	STMFD	sp!, {lr}
	BL	malloc
	CMP	a1, #0
	CMPNE	fp, #0
	[ ALLOCA_FATAL = 0
	stackreturn	EQ, "pc"
	|
	; If we could not malloc any space then print an error message
	; and force an abort - just like a true alloca function should.
	ADREQ	a1, |__alloca_malloc_msg|
	SWIEQ	XOS_Write0
	BEQ	abort		; never returns
	]

	LDR	a3, =|__alloca_list|
	LDR	a2, [fp, #-4]
	LDR	a4, [a3]
	STR	a2, [a1, #4]
	STR	a4, [a1]
	STR	a1, [a3]
	AND	a2, a2, #&fc000003
	ADR	a3, |__alloca_free|
	BIC	a3, a3, #&fc000003
	ORR	a2, a2, a3
	STR	a2, [fp, #-4]
	ADD	a1, a1, #8
	stackreturn	AL, "pc"

|__alloca_free|
	LDR	a3, =|__alloca_list|	; StrongARM order
	STMFD	sp!, {a1, a2, v1}
	LDR	a1, [a3]
	LDMIA	a1, {a4, v1}
	STR	a4, [a3]
	BL	free
	LDR	a2, =|__base|
	MOV	a3, v1
	LDR	a2, [a2]
	CMP	a3, a2
	BCC	|__alloca_rtn_corrupt|
	BIC	a2, a3, #3
	CMP	a3, a2
	BNE	|__alloca_rtn_corrupt|
	LDMFD	sp!, {a1, a2, v1}
	return	AL, pc, a3

	[ ALLOCA_FATAL = 1
|__alloca_malloc_msg|
	DCB	"could not claim enough space for alloca"
	DCB	13, 10, 0
	]

|__alloca_rtn_msg|
	DCB	"return from alloca() not possible, return point corrupted"
	DCB	13, 10, 0
	ALIGN

|__alloca_rtn_corrupt|
	ADR	a1, |__alloca_rtn_msg|
	SWI	XOS_Write0
	B	abort		; never returns


	AREA	|C$$zidata|, DATA, NOINIT

	EXPORT  |__alloca_list|
|__alloca_list|
	%	4

	END

