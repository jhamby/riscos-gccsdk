;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosfind.s,v $
; $Date: 2000/07/15 14:52:29 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_osfind (int op, char *name)
	IMPORT	|__seterr|
	EXPORT	|_kernel_osfind|
|_kernel_osfind|
	STMFD	sp!, {lr}
	SWI	XOS_Find
	stackreturn	VC, "pc"
	BL	|__seterr|
	MVN	a1, #1
	stackreturn	AL, "pc"

	END
