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

	AREA	|C$$code|, CODE, READONLY

	IMPORT  malloc
	IMPORT  free
	IMPORT  |__base|
	IMPORT  abort

	EXPORT  |__alloca|
	EXPORT  alloca
	NAME	__alloca
alloca		; just in case
|__alloca|
	CMP	a1, #0
	return	EQ, pc, lr

	ADD	a1, a1, #8
	STMFD	sp!, {lr}
	BL	malloc
	CMP	a1, #0
	CMPNE	fp, #0			;FIXME: if fp = NULL, we have a memory leak
	[ ALLOCA_FATAL = 0
	stackreturn	EQ, "pc"
	|
	; If we could not malloc any space then print an error message
	; and force an abort - just like a true alloca function should.
	ADREQ	a1, |__alloca_malloc_msg|
	SWIEQ	XOS_Write0
	BEQ	abort			; never returns
	]

	; Overwrite the return link value of the caller by __alloca_free which will
	; free the malloc'd block and call the original return link value.

	; + 0 : original __alloca_list value
	; + 4 : return link value of caller (i.e. [fp, #-4])
	; + 8 : start contents block returned from alloca()

	LDR	a3, =|__alloca_list|
	LDR	a2, [fp, #-4]
	LDR	a4, [a3]
	STR	a2, [a1, #4]
	STR	a4, [a1, #0]
	STR	a1, [a3]		; Add the malloc'd block in front of the __alloca_list

	[ {CONFIG} = 26
	AND	a2, a2, #&fc000003	; a2 = PSR flags
	ADR	a3, |__alloca_free|
	BIC	a3, a3, #&fc000003
	ORR	a2, a2, a3
	|
	ADR	a3, |__alloca_free|
	]
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
