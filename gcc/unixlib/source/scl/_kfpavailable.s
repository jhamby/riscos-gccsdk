; _kernel_fpavailable
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; int _kernel_fpavailable (void)
	; Return non-zero if the floating point instruction set is available.
	; It is a bit dubious what to return here in case of soft-float builds.
	EXPORT	|_kernel_fpavailable|
	NAME	_kernel_fpavailable
|_kernel_fpavailable|
	MOV	a1, #1
	MOV	pc, lr
	DECLARE_FUNCTION _kernel_fpavailable

	END
