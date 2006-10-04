; __umodsi3
; GCC compatability file
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|_kernel_udiv|

	EXPORT	|__umodsi3|
	NAME	__umodsi3
|__umodsi3|
	STMFD	sp!, {lr}
	MOV	a3, a1
	MOV	a1, a2
	MOV	a2, a3
	BL	|_kernel_udiv|
	MOV	a1, a2
	LDMFD	sp!, {pc}

	END
