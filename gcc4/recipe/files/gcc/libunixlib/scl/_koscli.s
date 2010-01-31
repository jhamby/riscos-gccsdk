@ _kernel_oscli
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int _kernel_oscli (const char *cli)
	.global	_kernel_oscli
	NAME	_kernel_oscli
_kernel_oscli:
	STMFD	sp!, {lr}
	SWI	XOS_CLI
	MOVVC	a1, #1
	LDMVCFD	sp!, {pc}

	MOV	a2, #0
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_oscli

	.end
