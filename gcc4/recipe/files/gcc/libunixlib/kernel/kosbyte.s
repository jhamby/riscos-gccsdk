@ _kernel_osbyte
@ Copyright (c) 2000-2010 UnixLib Developers

#include "internal/asm_dec.s"

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
	MOV	a2, #EOPSYS
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_osbyte

	.end
