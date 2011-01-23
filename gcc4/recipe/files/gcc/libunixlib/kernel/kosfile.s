@ _kernel_osfile
@ Copyright (c) 2000-2011 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int _kernel_osfile (int op, char *name, _kernel_osfile_block *inout)
	.global	_kernel_osfile
	NAME	_kernel_osfile
_kernel_osfile:
	@ don't need v3, but it can be set with OS_File 20-23
	STMFD	sp!, {v1-v3, lr}
	MOV	ip, a3
	LDMIA	a3, {a3, a4, v1, v2}
	SWI	XOS_File
	STMIA	ip, {a3, a4, v1, v2}
	LDMVCFD	sp!, {v1-v3, pc}

	MOV	a2, #EOPSYS
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {v1-v3, pc}
	DECLARE_FUNCTION _kernel_osfile

	.end
