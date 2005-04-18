;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosrdch.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__ul_seterr|

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
	MOV	a2, #1
	BL	|__ul_seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
