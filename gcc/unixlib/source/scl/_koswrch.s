;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_koswrch.s,v $
; $Date: 2000/07/15 14:52:29 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_oswrch (int ch)
	IMPORT	|__seterr|
	EXPORT	|_kernel_oswrch|
|_kernel_oswrch|
	STMFD	sp!, {lr}
	SWI	XOS_WriteC
	stackreturn	VC, "pc"
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "pc"

	END
