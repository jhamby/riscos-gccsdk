@ _kernel_osfind
@ Copyright (c) 2000-2010 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int _kernel_osfind (int op, const char *name)
	.global	_kernel_osfind
	NAME	_kernel_osfind
_kernel_osfind:
	STMFD	sp!, {lr}
	SWI	XOS_Find
	LDMVCFD	sp!, {pc}

	MOV	a2, #EOPSYS
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}
	DECLARE_FUNCTION _kernel_osfind

	.end
