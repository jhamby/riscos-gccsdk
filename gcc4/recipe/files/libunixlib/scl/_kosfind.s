@ _kernel_osfind
@ Copyright (c) 2000-2007 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	@ int _kernel_osfind (int op, char *name)
	.global	_kernel_osfind
	NAME	_kernel_osfind
_kernel_osfind:
	STMFD	sp!, {lr}
	SWI	XOS_Find
	LDMVCFD	sp!, {pc}

	MOV	a2, #0
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_osfind

	.end
