@ _kernel_osbput
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	@ int _kernel_osbput (int ch, int handle)
	.global	_kernel_osbput
	NAME	_kernel_osbput
_kernel_osbput:
	SWI	XOS_BPut
	MOVVS	a1, #-2
	MOVVC	a1, #0
	MOV	pc, lr

	.end
