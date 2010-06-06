@ _kernel_getenv
@ Copyright (c) 2000-2010 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ _kernel_getenv (const char *name, char *buffer, unsigned size)
	.global	_kernel_getenv
	NAME	_kernel_getenv
_kernel_getenv:
	STMFD	sp!, {v1, lr}
	SUB	a3, a3, #1		@ leave room for terminator
	MOV	a4, #0
	MOV	v1, #3
	SWI	XOS_ReadVarVal
	MOVVC	a1, #0	@ Null terminate the string
	STRVCB	a1, [a2, a3]
	LDMVCFD	sp!, {v1, pc}

	MOV	v1, a1
	MOV	a2, #EOPSYS
	BL	__ul_seterr
	MOV	a1, v1
	LDMFD	sp!, {v1, pc}
	DECLARE_FUNCTION _kernel_getenv

	.end
