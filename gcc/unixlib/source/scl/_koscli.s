; _kernel_oscli
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__ul_seterr|

	; int _kernel_oscli (const char *cli)
	EXPORT	|_kernel_oscli|
	NAME	_kernel_oscli
|_kernel_oscli|
	STMFD	sp!, {lr}
	SWI	XOS_CLI
	MOVVC	a1, #1
	LDMVCFD	sp!, {pc}
	MOV	a2, #0
	BL	|__ul_seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
