;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kgetenv.s,v $
; $Date: 2000/07/15 14:52:28 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_getenv (const char *name, char *buffer, unsigned size)
	IMPORT	|__seterr|
	EXPORT	|_kernel_getenv|
|_kernel_getenv|
	STMFD	sp!, {v1, lr}
	SUB	a3, a3, #1		; leave room for terminator
	MOV	a4, #0
	MOV	v1, #3
	SWI	XOS_ReadVarVal
	MOVVC	a1, #0	; Null terminate the string
	STRVCB	a1, [a2, a3]
	stackreturn	VC, "v1, pc"

	MOV	v1, a1
	BL	|__seterr|
	MOV	a1, v1
	stackreturn	AL, "v1, pc"

	END
