;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosfind.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_osfind (int op, char *name)
	EXPORT	|_kernel_osfind|
	NAME	_kernel_osfind
|_kernel_osfind|
	STMFD	sp!, {lr}
	SWI	XOS_Find
	stackreturn	VC, "pc"
	BL	|__seterr|
	MVN	a1, #1
	stackreturn	AL, "pc"

	END
