; __modsi3
; GCC compatability file
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|_kernel_sdiv|

	EXPORT	|__modsi3|
	NAME	__modsi3
|__modsi3|
	STMFD	sp!, {lr}
	MOV	a3, a1
	MOV	a1, a2
	MOV	a2, a3
	BL	|_kernel_sdiv|
	MOV	a1, a2
	LDMFD	sp!, {pc}

	END
