;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_koscli,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
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
