@ _kernel_osrdch
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text


	@ int _kernel_osrdch (void)
	.global	_kernel_osrdch
	NAME	_kernel_osrdch
_kernel_osrdch:
	STMFD	sp!, {lr}
	SWI	XOS_ReadC
	BVS	error
	LDMCCFD	sp!, {pc}
	@ Escape has been seen.
	TEQ	a1, #27
	MOVEQ	a1, #-27
	MOVNE	a1, #-1
	LDMFD	sp!, {pc}

error:
	MOV	a2, #1
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	.end
