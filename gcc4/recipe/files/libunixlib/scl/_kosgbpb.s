@ _kernel_osgbpb
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int _kernel_osgbpb (int op, unsigned handle,
	@		      _kernel_osgbpb_block *inout)
	.global	_kernel_osgbpb
	NAME	_kernel_osgbpb
_kernel_osgbpb:
	STMFD	sp!, {v1, v2, v3, v4, lr}
	MOV	v4, a3
	LDMIA	v4, {a3, a4, v1, v2, v3}
	SWI	XOS_GBPB
	BVS	_kernel_osgbpb_err
	STMIA	v4, {a3, a4, v1, v2, v3}
	MVNCS	a1, #0
	LDMCSFD	sp!, {v1, v2, v3, v4, pc}

_kernel_osgbpb_err:
	MOV	a2, #0
	BL	__ul_seterr
	MOV	a1, #-2
	LDMFD	sp!, {v1, v2, v3, v4, pc}
	DECLARE_FUNCTION _kernel_osgbpb

	.end
