;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosrdch.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
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
	LDMCCFD	sp!, {pc}
	; Escape has been seen.
	TEQ	a1, #27
	MOVEQ	a1, #-27
	MOVNE	a1, #-1
	LDMFD	sp!, {pc}

error
	BL	|__seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
