@ _kernel_osbput
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int _kernel_osbput (int ch, int handle)
	.global	_kernel_osbput
	NAME	_kernel_osbput
_kernel_osbput:
	STMFD	sp!, {lr}
	SWI	XOS_BPut
	MOVVC	a1, #0
	LDMVCFD	sp!, {pc}

	MOV	a2, #0
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_osbput

	.end
