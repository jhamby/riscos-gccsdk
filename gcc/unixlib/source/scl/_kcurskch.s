;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_kcurskch,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_stack_chunk *_kernel_current_stack_chunk (void)
	EXPORT	|_kernel_current_stack_chunk|
|_kernel_current_stack_chunk|
	SUB	a1, sl, #560
	LDR	a2, =&F60690FF
	LDR	a3, [a1, #0]
	CMP	a2, a3
	MOVNE	a1, #0
	return	AL, pc, lr

	END
