;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_koscli.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_oscli (char *cli)
	EXPORT	|_kernel_oscli|
	NAME	_kernel_oscli
|_kernel_oscli|
	STMFD	sp!, {v6, lr}
	SWI	XOS_CLI
	MOVVC	a1, #1
	stackreturn	VC, "v6, pc"
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "v6, pc"

	END
