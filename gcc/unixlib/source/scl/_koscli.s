;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_koscli.s,v $
; $Date: 2000/07/15 14:52:29 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_oscli (char *cli)
	IMPORT	|__seterr|
	EXPORT	|_kernel_oscli|
|_kernel_oscli|
	STMFD	sp!, {v6, lr}
	SWI	XOS_CLI
	MOVVC	a1, #1
	stackreturn	VC, "v6, pc"
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "v6, pc"

	END
