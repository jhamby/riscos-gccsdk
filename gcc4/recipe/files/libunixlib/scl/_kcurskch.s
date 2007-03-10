@ _kernel_current_stack_chunk
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	@ _kernel_stack_chunk *_kernel_current_stack_chunk (void)
	.global	_kernel_current_stack_chunk
	NAME	_kernel_current_stack_chunk
_kernel_current_stack_chunk:
	SUB	a1, sl, #560
	LDR	a2, =0xF60690FF
	LDR	a3, [a1, #0]
	TEQ	a2, a3
	MOVNE	a1, #0
	MOV	pc, lr

	.end
