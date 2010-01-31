@ _kernel_fpavailable
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ int _kernel_fpavailable (void)
	@ Return non-zero if the floating point instruction set is available.
	@ It is a bit dubious what to return here in case of soft-float builds.
	.global	_kernel_fpavailable
	NAME	_kernel_fpavailable
_kernel_fpavailable:
	MOV	a1, #1
	MOV	pc, lr
	DECLARE_FUNCTION _kernel_fpavailable

	.end
