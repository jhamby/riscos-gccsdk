;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kcurskch.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

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
