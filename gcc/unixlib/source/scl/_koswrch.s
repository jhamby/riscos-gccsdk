;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_koswrch.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_oswrch (int ch)
	EXPORT	|_kernel_oswrch|
	NAME	_kernel_oswrch
|_kernel_oswrch|
	STMFD	sp!, {lr}
	SWI	XOS_WriteC
	LDMVCFD	sp!, {pc}
	BL	|__seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
