@ _kernel_current_stack_chunk
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ _kernel_stack_chunk *_kernel_current_stack_chunk (void)
	.global	_kernel_current_stack_chunk
	NAME	_kernel_current_stack_chunk
_kernel_current_stack_chunk:
	SUB	a1, sl, #512 + CHUNK_OVERHEAD
	LDR	a2, =0xF60690FF
	LDR	a3, [a1, #CHUNK_MAGIC]
	TEQ	a2, a3
	MOVNE	a1, #0
	MOV	pc, lr
	DECLARE_FUNCTION _kernel_current_stack_chunk

	.end
