;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosrdch.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; int _kernel_osrdch (void)
	EXPORT	|_kernel_osrdch|
	NAME	_kernel_osrdch
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
