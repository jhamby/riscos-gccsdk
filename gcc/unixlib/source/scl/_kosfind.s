;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_kosfind,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
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
