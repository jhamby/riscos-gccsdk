@ _kernel_osfind
@ Copyright (c) 2000-2006 UnixLib Developers

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
	MVN	a1, #1
	LDMFD	sp!, {pc}

	.end
