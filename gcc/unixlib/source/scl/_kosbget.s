;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosbget.s,v $
; $Date: 2000/07/15 14:52:28 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; int _kernel_osbget (int handle)
	IMPORT	|__seterr|
	EXPORT	|_kernel_osbget|
|_kernel_osbget|
	STMFD	sp!, {lr}
	MOV	a2, a1
	SWI	XOS_BGet
	MOVCS	a1, #-1		; We have EOF. Return -1
	stackreturn	CS, "pc"

	BL	|__seterr|
	MOV	a1, #-2		; Error occurred. Return -2
	stackreturn	AL, "pc"

	END
