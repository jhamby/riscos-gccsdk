@ _kernel_osargs
@ Copyright (c) 2000-2010 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ _kernel_osargs (int op, unsigned handle, int arg)
	.global	_kernel_osargs
	NAME	_kernel_osargs
_kernel_osargs:
	STMFD	sp!, {lr}
	ORR	ip, a1, a2	@ ip = 0 iff op = handle = 0
	SWI	XOS_Args
	BVS	error
	TEQ	ip, #0
	MOVNE	a1, a3
	LDMFD	sp!, {pc}
error:
	MOV	a2, #EOPSYS
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_osargs

	.end
