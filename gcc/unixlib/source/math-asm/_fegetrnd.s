;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fegetrnd.s,v $
; $Date: 2000/07/15 14:52:23 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
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
