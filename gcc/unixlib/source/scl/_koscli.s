;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_koscli.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_oscli (const char *cli)
	EXPORT	|_kernel_oscli|
	NAME	_kernel_oscli
|_kernel_oscli|
	STMFD	sp!, {lr}
	SWI	XOS_CLI
	MOVVC	a1, #1
	stackreturn	VC, "pc"
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "pc"

	END
