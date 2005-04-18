;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosfind.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__ul_seterr|

	; int _kernel_osfind (int op, char *name)
	EXPORT	|_kernel_osfind|
	NAME	_kernel_osfind
|_kernel_osfind|
	STMFD	sp!, {lr}
	SWI	XOS_Find
	LDMVCFD	sp!, {pc}
	MOV	a2, #0
	BL	|__ul_seterr|
	MVN	a1, #1
	LDMFD	sp!, {pc}

	END
