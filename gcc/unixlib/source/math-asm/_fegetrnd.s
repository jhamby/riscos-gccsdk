;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_fegetrnd,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fegetround
fegetround
	; The default rounding mode cannot be set on the ARM FPU without
	; handcoding the rounding mode into the instruction. We therefore
	; always default to round-to-nearest.
	mov	a1, #0
	return	AL, pc, lr

	END
