;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_kosrdch,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; int _kernel_osrdch (void)
	IMPORT	|__seterr|
	EXPORT	|_kernel_osrdch|
|_kernel_osrdch|
	STMFD	sp!, {lr}
	SWI	XOS_ReadC
	BVS	error
	stackreturn	CC, "pc"
	; Escape has been seen.
	CMP	a1, #27
	MOVEQ	a1, #-27
	MOVNE	a1, #-1
	stackreturn	AL, "pc"

error
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "pc"

	END
