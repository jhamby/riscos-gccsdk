; _kernel_osbput
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; int _kernel_osbput (int ch, int handle)
	EXPORT	|_kernel_osbput|
	NAME	_kernel_osbput
|_kernel_osbput|
	SWI	XOS_BPut
	MOVVS	a1, #-2
	MOVVC	a1, #0
	MOV	pc, lr

	END
