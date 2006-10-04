; _kernel_osbyte
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__ul_seterr|

	; int _kernel_osbyte (int op, int x, int y)
	EXPORT	|_kernel_osbyte|
	NAME	_kernel_osbyte
|_kernel_osbyte|
	STMFD	sp!, {lr}
	SWI	XOS_Byte
	BVS	|err|
	AND	a1, a2, #&FF
	AND	a3, a3, #&FF
	ORR	a1, a1, a3, LSL #8
	; If carry set, then third byte = 1
	ORRCS	a1, a1, #1:SHL:16
	LDMFD	sp!, {pc}
|err|
	MOV	a2, #0
	BL	|__ul_seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
