;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosfind.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; int _kernel_osfind (int op, char *name)
	EXPORT	|_kernel_osfind|
	NAME	_kernel_osfind
|_kernel_osfind|
	STMFD	sp!, {lr}
	SWI	XOS_Find
	LDMVCFD	sp!, {pc}
	BL	|__seterr|
	MVN	a1, #1
	LDMFD	sp!, {pc}

	END
