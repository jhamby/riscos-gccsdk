@ _kernel_osbyte
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text


	@ int _kernel_osbyte (int op, int x, int y)
	.global	_kernel_osbyte
	NAME	_kernel_osbyte
_kernel_osbyte:
	STMFD	sp!, {lr}
	SWI	XOS_Byte
	BVS	err
	AND	a1, a2, #0xFF
	AND	a3, a3, #0xFF
	ORR	a1, a1, a3, LSL #8
	@ If carry set, then third byte = 1
	ORRCS	a1, a1, #1<<16
	LDMFD	sp!, {pc}
err:
	MOV	a2, #0
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	.end
