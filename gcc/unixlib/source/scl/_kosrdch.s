;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosrdch.s,v $
; $Date: 2000/07/15 14:52:29 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
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
