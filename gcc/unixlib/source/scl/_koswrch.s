;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_koswrch.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_oswrch (int ch)
	EXPORT	|_kernel_oswrch|
	NAME	_kernel_oswrch
|_kernel_oswrch|
	STMFD	sp!, {lr}
	SWI	XOS_WriteC
	stackreturn	VC, "pc"
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "pc"

	END
