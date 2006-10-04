; _kernel_osbget
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__ul_seterr|

	; int _kernel_osbget (int handle)
	EXPORT	|_kernel_osbget|
	NAME	_kernel_osbget
|_kernel_osbget|
	STMFD	sp!, {lr}
	MOV	a2, a1
	SWI	XOS_BGet
	MOVCS	a1, #-1		; We have EOF. Return -1
	LDMCSFD	sp!, {pc}

	MOV	a2, #1
	BL	|__ul_seterr|
	MOV	a1, #-2		; Error occurred. Return -2
	LDMFD	sp!, {pc}

	END
