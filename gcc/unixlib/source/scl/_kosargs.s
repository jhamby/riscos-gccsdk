;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosargs.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_osargs (int op, unsigned handle, int arg)
	EXPORT	|_kernel_osargs|
	NAME	_kernel_osargs
|_kernel_osargs|
	STMFD	sp!, {lr}
	ORR	ip, a1, a2	; ip = 0 iff op = handle = 0
	SWI	XOS_Args
	BVS	error
	TEQ	ip, #0
	MOVNE	a1, a3
	LDMFD	sp!, {pc}
error
	BL	|__seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
