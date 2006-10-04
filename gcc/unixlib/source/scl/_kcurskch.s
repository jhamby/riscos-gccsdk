; _kernel_current_stack_chunk
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_stack_chunk *_kernel_current_stack_chunk (void)
	EXPORT	|_kernel_current_stack_chunk|
	NAME	_kernel_current_stack_chunk
|_kernel_current_stack_chunk|
	SUB	a1, sl, #560
	LDR	a2, =&F60690FF
	LDR	a3, [a1, #0]
	TEQ	a2, a3
	MOVNE	a1, #0
	MOV	pc, lr

	END
