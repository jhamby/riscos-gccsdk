;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosword.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_osword (int op, int *data)
	EXPORT	|_kernel_osword|
	NAME	_kernel_osword
|_kernel_osword|
	STMFD	sp!, {lr}
	SWI	XOS_Word
	MOVVC	a1, a2
	stackreturn	VC, "pc"
	BL	|__seterr|
	MVN	a1, #1
	stackreturn	AL, "pc"

	END
