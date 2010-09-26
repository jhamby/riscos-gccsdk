@ _kernel_current_stack_chunk
@ Copyright (c) 2000-2010 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ _kernel_stack_chunk *_kernel_current_stack_chunk (void)
	.global	_kernel_current_stack_chunk
	NAME	_kernel_current_stack_chunk
_kernel_current_stack_chunk:
#if __UNIXLIB_CHUNKED_STACK
	SUB	a1, sl, #512 + CHUNK_OVERHEAD
	LDR	a2, =0xF60690FF
	LDR	a3, [a1, #CHUNK_MAGIC]
	TEQ	a2, a3
	MOVNE	a1, #0
#else
	@ We don't have a chunked stack:
	MOV	a1, #0
#endif
	MOV	pc, lr
	DECLARE_FUNCTION _kernel_current_stack_chunk

	.end
