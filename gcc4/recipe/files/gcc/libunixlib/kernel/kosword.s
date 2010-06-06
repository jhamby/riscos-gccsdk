@ _kernel_osword
@ Copyright (c) 2000-2010 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int _kernel_osword (int __operation, int *__data)
	.global	_kernel_osword
	NAME	_kernel_osword
_kernel_osword:
	STMFD	sp!, {lr}
	SWI	XOS_Word
	MOVVC	a1, #0
	LDMVCFD	sp!, {pc}

	MOV	a2, #EOPSYS
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_osword

	.end
