;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_koswrch,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
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
