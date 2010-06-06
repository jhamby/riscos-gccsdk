@ _kernel_osbget
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int _kernel_osbget (int handle)
	.global	_kernel_osbget
	NAME	_kernel_osbget
_kernel_osbget:
	STMFD	sp!, {lr}
	MOV	a2, a1
	SWI	XOS_BGet
	MOVCS	a1, #-1		@ We have EOF. Return -1
	LDMCSFD	sp!, {pc}

	MOV	a2, #EOPSYS
	BL	__ul_seterr
	MOV	a1, #-2		@ Error occurred. Return -2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_osbget

	.end
