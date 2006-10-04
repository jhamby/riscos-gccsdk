; _kernel_osargs
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__ul_seterr|

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
	MOV	a2, #0
	BL	|__ul_seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
