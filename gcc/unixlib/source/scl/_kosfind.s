; _kernel_osfind
; Copyright (c) 2000-2006 UnixLib Developers

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
