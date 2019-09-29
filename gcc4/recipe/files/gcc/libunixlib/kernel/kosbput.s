@ _kernel_osbput
@ Copyright (c) 2000-2010 UnixLib Developers

#include "internal/asm_dec.s"

	.syntax unified
	.text

	@ int _kernel_osbput (int ch, int handle)
	.global	_kernel_osbput
	NAME	_kernel_osbput
_kernel_osbput:
	STMFD	sp!, {lr}
	SWI	XOS_BPut
	MOVVC	a1, #0
	LDMFDVC	sp!, {pc}

	MOV	a2, #EOPSYS
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_osbput

	.end
