;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_kosword,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_osword (int op, int *data)
	IMPORT	|__seterr|
	EXPORT	|_kernel_osword|
|_kernel_osword|
	STMFD	sp!, {lr}
	SWI	XOS_Word
	MOVVC	a1, a2
	stackreturn	VC, "pc"
	BL	|__seterr|
	MVN	a1, #1
	stackreturn	AL, "pc"

	END
