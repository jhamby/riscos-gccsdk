;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_ksetenv.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_setenv (const char *name, const char *value)
	EXPORT	|_kernel_setenv|
	NAME	_kernel_setenv
|_kernel_setenv|
	STMFD	sp!, {v1, lr}
	SUB	a3, a2, #1
	CMP	a2, #0
	; Get length of 'value' if not NULL
|loop|
	LDRNEB	ip, [a3,#1]!
	CMPNE	ip, #0
	BNE	|loop|
	SUB	a3, a3, a2		; length, or -1 if value = NULL
	MOV	a4, #0
	MOV	v1, #0
	SWI	XOS_SetVarVal
	MOVVC	a1, #0
	stackreturn	VC, "v1, pc"
	MOV	v1, a1
	BL	|__seterr|
	MOV	a1, v1
	stackreturn	AL, "v1, pc"

	END
